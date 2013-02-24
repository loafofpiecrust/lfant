/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-19 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/

#include "Network.hpp"

// External
#include <SocketLayer.h>
#include <Kbhit.h>
#include <PacketLogger.h>
#include <BitStream.h>
#include <RakSleep.h>
#include <MessageIdentifiers.h>
#include <RakNetTypes.h>
#include <RakNetStatistics.h>

// Internal
#include "Console.hpp"
#include "Thread.hpp"

using namespace RakNet;

namespace sfs
{

Network::Network()
{
	//Init();
}

Network::~Network()
{
}

Connection::Connection(string name, string host, uint16_t port, string password, uint16_t maxPeers) :
	name(name), host(host), password(password), socket(port, 0), maxPeers(maxPeers)
{
	socket.socketFamily = AF_INET;
	//peer = new RakPeer();
	peer = RakPeerInterface::GetInstance();
	Log("Connection::Connection: Created peer instance");
	peer->AttachPlugin(&fcm);
	peer->AttachPlugin(&cg);
	fcm.SetAutoparticipateConnections(true);
	// Attach any more plugins you want here.
}

void Network::Init()
{
	//	Connect("Game", "10.0.1.22", 1234, "", 64);
	Connect("Chat", "174.127.119.2", 1234, "", 16);
	Ping("Chat");
}

void Network::Update()
{

	sleep(20);
	Packet* packet = nullptr;
	for(Connection* con : connections)
	{
		if(con)
		{
			continue;
		}
		if(!con->started || !con->peer->IsActive())
		{
			continue;
		}
		packet = con->peer->Receive();
		auto packetId = GetPacketId(packet);
		switch(packetId)
		{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				Log("Network::Update: DISCONNECTION_NOTIFICATION");
				break;

			case ID_NEW_INCOMING_CONNECTION:
				// Somebody connected.  We have their IP now
				Log("Network::Update: NEW_INCOMING_CONNECTION");
				Log("Network::Update: Its from ", packet->systemAddress.ToString(true), ", guid=", packet->guid.ToString());
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				//packet = con->peer->
				// Somebody connected.  We have their IP now
				Log("Network::Update: CONNECTION_ACCEPTED");
				Log("Network::Update: Its from ", packet->systemAddress.ToString(true), ", guid=", packet->guid.ToString());
				if(packet->guid != con->peer->GetMyGUID())
				{
					con->peer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(), 0, 0);
				}
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				Log("Network::Update: CONNECTION_LOST");
				break;

			case ID_ADVERTISE_SYSTEM:
				Log("Network::Update: ADVERTISE_SYSTEM");
				// Do something here?
				break;

			case ID_FCM2_NEW_HOST:
				Log("Network::Update: NEW_HOST");
				if(packet->guid == con->peer->GetMyGUID())
				{
					Log("Got new host (ourselves)");	// You're the new host, do something with that
				}
				else
				{
					Log("Got new host ", packet->systemAddress.ToString(true), ", GUID=", packet->guid.ToString());  // Someone else is host, GUI!
				}
				BitStream bs(packet->data, packet->length, false);
				bs.IgnoreBytes(1);
				RakNetGUID oldHost;
				bs.Read(oldHost);
				// If oldHost is different then the current host, then we lost connection to the host
				if(oldHost != UNASSIGNED_RAKNET_GUID)
				{
					Log(". Old-host Guid=", oldHost.ToString());	// Host is diff, this is old
				}
				else
				{
					Log("(First reported host)");   // The host is the same
				}
				break;
		}
		con->peer->DeallocatePacket(packet);
	}

}

bool Network::Connect(string name, string host, uint16_t port, string password, uint16_t maxPeers)
{
	Log("Network::Connect: Begin");
	Connection* con = nullptr;
	bool isnew = false;
	if(auto pco = GetConnection(name))
	{
		con = pco;
	}
	if(!con)
	{
		Log("Network::Connect: About to spawn.");
		con = new Connection(name, host, port, password, maxPeers);
		connections.push_back(con);
		isnew = true;
	}

	if(!isnew)
	{
		if(con->host == host && con->password == password && con->socket.port == port && con->maxPeers == maxPeers)
		{
			return true;
		}
		con->host = host;
		con->socket.port = port;
		con->password = password;
		con->maxPeers = maxPeers;
	}

	Log("Network::Connect: New connection, "+con->name+", at "+con->host+" ", con->socket.port, ", max peers: ", con->maxPeers);
	while(IRNS2_Berkley::IsPortInUse(con->socket.port, con->socket.hostAddress, con->socket.socketFamily, SOCK_DGRAM) == true)
	{
		con->socket.port++;
	}

	Log("Network::Connect: Port not in use");

	if(con->started)
	{
		Disconnect(name);
	}

	if(con->peer->Startup(con->maxPeers, &con->socket, 1) != RAKNET_STARTED)
	{
		Disconnect(name);
		return false;
	}
	con->started = true;
	con->peer->SetOccasionalPing(true);
	con->peer->SetMaximumIncomingConnections(con->maxPeers);

	Log("Our guid is ", con->peer->GetGuidFromSystemAddress(RakNet::UNASSIGNED_SYSTEM_ADDRESS).ToString());
	Log("Started on ", con->peer->GetMyBoundAddress().ToString(true));
	con->peer->SetTimeoutTime(1000, UNASSIGNED_SYSTEM_ADDRESS);

	Log("Network::Connect: Peer connection opened");

	bool b = false;
	if(password == "")
	{
		b = con->peer->Connect(host.c_str(), con->socket.port, 0, 0) == CONNECTION_ATTEMPT_STARTED;
	}
	else
	{
		con->peer->SetIncomingPassword(password.c_str(), password.size()*sizeof(char));
		Log("Network::Connect: Password set");
		b = con->peer->Connect(host.c_str(), con->socket.port, password.c_str(), strlen(password.c_str())) == CONNECTION_ATTEMPT_STARTED;
	}
	if(!b)
	{
		// Something happened, can't connect
		Log("Network::Connect: Something happened, can't connect");
		Disconnect(name);
		return false;
	}
	Log("Network::Connect: Connected!");
	return true;
}

void Network::Disconnect(string name)
{
	if(auto con = GetConnection(name))
	{
		con->peer->CloseConnection(con->peer->GetSystemAddressFromIndex(0), false);
		con->peer->Shutdown(100);
		con->started = false;
	}
}

void Network::Ping(string name)
{
	if(Connection* con = GetConnection(name))
	{
		Log("About to ping ", con->peer->GetSystemAddressFromIndex(0).ToString());
		if(con->peer->GetSystemAddressFromIndex(0) != RakNet::UNASSIGNED_SYSTEM_ADDRESS)
		{
			Log("PINGED!");
			con->peer->Ping(con->peer->GetSystemAddressFromIndex(0));
			Log("Average ping: ", con->peer->GetAveragePing(con->peer->GetSystemAddressFromIndex(0)));
		}
	}
}

Connection* Network::GetConnection(string name)
{
	for(auto con : connections)
	{
		if(con->name == name)
		{
			return con;
		}
	}
	return nullptr;
}

byte Network::GetPacketId(Packet* p)
{
	if (p == nullptr)
		return 255;

	if ((byte)p->data[0] == ID_TIMESTAMP)
	{
		RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
		return (byte) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
		return (byte) p->data[0];
}

}

