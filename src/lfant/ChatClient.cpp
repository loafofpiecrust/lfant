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

void ChatClient::SendMessage(string msg)
{
	client->SendData(msg);
	lastMsg = msg;
}

void ChatClient::ReceiveMessage(string msg)
{
	Log("Received message: '"+msg+"'\n");
	lastMsg = msg;
}

void ChatClient::OnDestroy()
{
	delete client;
}

void ChatClient::OnConnect(string error)
{
	if(error == "")
	{
		Log("ChatClient::OnConnect: Success.");
		client->SendData(":/client "+name+" "+client->socket.remote_endpoint().address().to_string());
		client->GetDataAsync(256);
	}
	else
	{
		// Connect failed, let's host!
		Host();
	}
}

void ChatClient::OnHost(string error)
{
	Log("Started hosting.");
	if(error == "")
	{

	}
	TriggerEvent("Host");
}

void ChatClient::OnGetData(string data)
{
	if(data.size() > 1 && data[0] == ':' && data[1] == '/')
	{
		// Calling a command
		data.erase(data.begin(), data.begin()+2);
		deque<string> toks = Split(data, " ");
		if(toks[0] == "client")
		{
			Peer peer;
			peer.name = toks[1];
			peer.ip = toks[2];
			peers.push_back(peer);
		}
		return;
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
