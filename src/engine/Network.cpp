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

using namespace sfs;
using namespace RakNet;

Network::Network()
{
	//Init();
}

Network::~Network()
{
}

void Network::Init()
{
	gamePeer = new Connection(1234);
	chatPeer = new Connection(60000);
}

void Network::Update()
{
}

bool Network::Setup(Connection* con)
{
	while(SocketLayer::IsPortInUse(con->socket.port, con->socket.hostAddress, con->socket.socketFamily) == true)
	{
		con->socket.port++;
	}

	bool success = con->peer->Startup(con->maxPeers, &con->socket, 1) == RAKNET_STARTED;

	if(!success)
	{
		con->ready = false;
		return false;
	}

	con->peer->SetMaximumIncomingConnections(64);
	con->peer->SetTimeoutTime(1000, UNASSIGNED_SYSTEM_ADDRESS);
	con->peer->SetOccasionalPing(true);
	con->peer->SetUnreliableTimeout(1000);

	con->ready = true;
	return true;
}

bool Network::Connect(Connection* con, string host, string password)
{
	if(!con->ready)
	{
		for(uint i = 0; i < 5; ++i)
		{
			if(Setup(con))
			{
				break;
			}
		}
		if(!con->ready)
		{
			return false;
		}
	}

	if(password != "")
	{
		con->peer->SetIncomingPassword(password.c_str(), password.size()*sizeof(char));
	}

	con->peer->SetOccasionalPing(true);
	bool b = con->peer->Connect(host.c_str(), con->socket.port, password.c_str(),
								strlen(password.c_str())) == CONNECTION_ATTEMPT_STARTED;
	if(!b)
	{
		// Something happened, can't connect
		Disconnect(con);
	}
	return b;
}

bool Network::ConnectFCM(Connection* con, string host,
						 string password)
{
	if(!con->ready)
	{
		for(uint i = 0; i < 5; ++i)
		{
			if(Setup(con))
			{
				break;
			}
		}
		if(!con->ready)
		{
			return false;
		}
	}

	while(SocketLayer::IsPortInUse(con->socket.port, con->socket.hostAddress, con->socket.socketFamily)
			== true)
	{
		++con->socket.port;
	}
	StartupResult sr = con->peer->Startup(con->maxPeers, &con->socket, 1);
	con->peer->SetMaximumIncomingConnections(con->maxPeers);
	con->peer->SetTimeoutTime(1000, UNASSIGNED_SYSTEM_ADDRESS);
	if(sr != RAKNET_STARTED)
	{
		Disconnect(con);
		return false;
	}

	// Loop to make sure each new client gets interconnected
	bool quit = false;
	Packet* packet;
	//	char ch;
	while(!quit)
	{
		for(packet = con->peer->Receive(); packet; con->peer->DeallocatePacket(packet), packet = con->peer->Receive())
		{
			switch(packet->data[0])
			{
				case ID_DISCONNECTION_NOTIFICATION:
					// Connection lost normally
					printf("ID_DISCONNECTION_NOTIFICATION\n");
					break;

				case ID_NEW_INCOMING_CONNECTION:
					// Somebody connected.  We have their IP now
					printf("ID_NEW_INCOMING_CONNECTION from %s. guid=%s.\n",
						   packet->systemAddress.ToString(true),
						   packet->guid.ToString());
					break;

				case ID_CONNECTION_REQUEST_ACCEPTED:
					// Somebody connected.  We have their IP now
					printf("ID_CONNECTION_REQUEST_ACCEPTED from %s. guid=%s.\n", packet->systemAddress.ToString(true), packet->guid.ToString());
					if(packet->guid != con->peer->GetMyGUID())
					{
						con->peer->Connect(packet->systemAddress.ToString(false), packet->systemAddress.GetPort(), 0, 0);
					}
					break;

				case ID_CONNECTION_LOST:
					// Couldn't deliver a reliable packet - i.e. the other system was abnormally
					// terminated
					printf("ID_CONNECTION_LOST\n");
					break;

				case ID_ADVERTISE_SYSTEM:
					// Do something here?
					break;

				case ID_FCM2_NEW_HOST:
					if(packet->guid == con->peer->GetMyGUID())
					{
						printf("Got new host (ourselves)");	// You're the new host, do something with that
					}
					else
					{
						printf("Got new host %s, GUID=%s", packet->systemAddress.ToString(true), packet->guid.ToString());  // Someone else is host, GUI!
					}
					BitStream bs(packet->data, packet->length, false);
					bs.IgnoreBytes(1);
					RakNetGUID oldHost;
					bs.Read(oldHost);
					// If oldHost is different then the current host, then we lost connection to the host
					if(oldHost != UNASSIGNED_RAKNET_GUID)
					{
						printf(". Old-host Guid=%s\n", oldHost.ToString());	// Host is diff, this is old
					}
					else
					{
						printf(". (First reported host)\n");   // The host is the same
					}
					break;
			}
		}
	}
	return true;
}

void Network::Disconnect(Connection* con)
{
//	con->fcm.SetAutoparticipateConnections(false);
	//Close any existing connection
	con->peer->CloseConnection(con->peer->GetSystemAddressFromIndex(0), false);
	// Turn off Raknet
	con->peer->Shutdown(100);
	// Revise this function to keep chat connection running?
}

