/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-02-24 by Taylor Snead
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

#include <lfant/ChatClient.h>

// External

// Internal
#include <lfant/network/Client.h>
#include <lfant/Network.h>
#include <lfant/Console.h>

namespace lfant
{

ChatClient::ChatClient()
{
}

ChatClient::~ChatClient()
{
}

void ChatClient::Init()
{
	client = new network::Client;
	//	connection = game->network->AddConnection<network::Client>(80);
	ConnectEvent(SENDER(client, Connect), RECEIVER(this, OnConnect));
	ConnectEvent(SENDER(client, Accept), RECEIVER(this, OnHost));
	ConnectEvent(SENDER(client, GetData), RECEIVER(this, OnGetData));
}

void ChatClient::Update()
{
	client->Update();
}

void ChatClient::SendMessage(string msg)
{
	msg = name+":"+msg;
	for(uint i = 0; i < peers.size(); ++i)
	{
		Log("Sending message to peer #", i);
		client->SendData(msg);
	}
	lastMsg = msg;
	Log("Message sent: '"+lastMsg+"'.");
}

void ChatClient::ReceiveMessage(string msg)
{
	string sender = "";
	uint pos = msg.find_first_of(':');
	if(pos != -1)
	{
		for(uint i = 0; i < pos; ++i)
		{
			sender.push_back(msg[i]);
		}
		msg.erase(msg.begin(), msg.begin()+pos+1);
	}
	Log("Received message: '"+msg+"' from '"+sender+"'.\n");
	lastMsg = msg;
}

void ChatClient::Disconnect()
{
	client->SendData(":\\client "+name+" "+client->socket.remote_endpoint().address().to_string());
	client->Disconnect();
}

void ChatClient::OnDestroy()
{
	Disconnect();
	delete client;
}

void ChatClient::OnConnect(string error)
{
	if(error == "")
	{
		Log("ChatClient::OnConnect: Success.");
		client->SendData(":/client "+name+" "+client->socket.remote_endpoint().address().to_string());
		client->GetDataAsync(256);
		TriggerEvent("Connect");
	}
}

void ChatClient::OnHost(string error)
{
	Log("Started hosting.");
	if(error == "")
	{
		client->GetDataAsync(256);
	}
	TriggerEvent("Host");
}

void ChatClient::OnGetData(string data)
{
	Log("ChatClient::OnGetData: Touch.");
	if(data.size() > 1 && data[0] == ':')
	{
		if(data[1] == '/')
		{
			// Calling a command
			data.erase(data.begin(), data.begin()+2);
			deque<string> toks = Split(data, " ");
			if(toks[0] == "client")
			{
				Log("ChatClient::OnGetData: Adding a peer.");
				Peer* peer = new Peer;
				peer->name = toks[1];
				peer->ip = toks[2];
				peers.push_back(*peer);

				client->SendData(":/server "+name+" "+client->socket.remote_endpoint().address().to_string());
			}
			else if(toks[0] == "server")
			{
				Log("ChatClient::OnGetData: Adding server as peer.");
				Peer* peer = new Peer;
				peer->name = toks[1];
				peer->ip = toks[2];
				peers.push_back(*peer);
			}
			return;
		}
		else if(data[1] == '\\')
		{
			// Calling a command
			data.erase(data.begin(), data.begin()+2);
			deque<string> toks = Split(data, " ");
			if(toks[0] == "client")
			{
				for(uint i = 0; i < peers.size(); ++i)
				{
					if(peers[i].name == toks[1] && peers[i].ip == toks[2])
					{
						peers.erase(peers.begin()+i);
					}
				}
			}
			return;
		}
	}

	ReceiveMessage(data);
}

void ChatClient::Connect()
{
	Log("Starting connection process...");
	/// @todo Use a legit IP, some specific, or method for determining?
	/// @todo Use a specific port.
	client->Connect("127.0.0.1", 22222);
}

void ChatClient::Host()
{
	Log("Starting hosting process...");
	client->Host(22222);
}

}
