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

// Internal
#include "Console.hpp"

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
	name(name), host(host), password(password), maxPeers(maxPeers)
{
	socket.port = port;
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
	Connect("Chat", "10.0.1.22", 60000, "", 16);
}

void Network::Update()
{
	/*
	Packet* packet = nullptr;
	for(Connection* con : connections)
	{
		Log("Network::Update: About to start packet checking");
		if(!con->started)
		{
			continue;
		}
		packet = con->peer->Receive();
		Log("Network::Update: Received packet");
		if(packet && packet->data)
		{
			Log("Network::Update: Packet data seems valid, size = ", sizeof(packet->data), ", first = ", (int)packet->data[0]);
			switch((unsigned int)packet->data[0])
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
			Log("Network::Update: About to deallocate packet");
			con->peer->DeallocatePacket(packet);
		}
		Log("Network::Update: Next connection!");
	}
	*/
}

bool Network::Connect(string name, string host, uint16_t port, string password, uint16_t maxPeers)
{
	Log("Network::Connect: Begin");
	Connection* con = nullptr;
	bool isnew = false;
	if(connections.size() > 0)
	{
		for(auto pco : connections)
		{
			if(pco->name == name)
			{
				con = pco;
				break;
			}
		}
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

	if(con->peer->Startup(con->maxPeers, &con->socket, 1, 1) != RAKNET_STARTED)
	{
		Disconnect(name);
		return false;
	}
	con->started = true;
	con->peer->SetMaximumIncomingConnections(con->maxPeers);
	con->peer->SetTimeoutTime(1000, UNASSIGNED_SYSTEM_ADDRESS);

	Log("Network::Connect: Peer connection opened");

	if(password != "")
	{
		con->peer->SetIncomingPassword(password.c_str(), password.size()*sizeof(char));
	}

	Log("Network::Connect: Password set");

	con->peer->SetOccasionalPing(true);
	bool b = con->peer->Connect(host.c_str(), con->socket.port, password.c_str(), strlen(password.c_str())) == CONNECTION_ATTEMPT_STARTED;
	if(!b)
	{
		// Something happened, can't connect
		Disconnect(name);
	}
	Log("Network::Connect: Connected!");
	return b;
}

void Network::Disconnect(string name)
{
	for(auto con : connections)
	{
		if(con->name == name)
		{
			con->peer->CloseConnection(con->peer->GetSystemAddressFromIndex(0), false);
			con->peer->Shutdown(100);
			con->started = false;
		}
	}
}

}

