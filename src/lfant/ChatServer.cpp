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

#include <lfant/ChatServer.h>

// External

// Internal
#include <lfant/Console.h>
#include <lfant/ChatClient.h>
#include <lfant/SystemInfo.h>

namespace lfant
{

IMPLEMENT_TYPE(net::Connection, ChatServer)

ChatServer::ChatServer()
{
	name = game->systemInfo->computerName;
}

ChatServer::~ChatServer()
{

}

void ChatServer::Init()
{
	ConnectEvent(SENDER(this, AddClient), RECEIVER(this, OnAddClient));
}

void ChatServer::OnGetData(const boost::system::error_code &error)
{
	OnGetData(error, &socket);
}

void ChatServer::OnGetData(const boost::system::error_code &error, asio::ip::tcp::socket *sock)
{
	Server::OnGetData(error, sock);
	string data = lastData;
	Log("ChatServer::OnGetData: Touch.");

	string sender = "";
	uint pos = data.find_first_of(':');
	if(pos != -1)
	{
		for(uint i = 0; i < pos; ++i)
		{
			sender.push_back(data[i]);
		}
		data.erase(data.begin(), data.begin()+pos+1);
	}

	deque<string> toks = Split(data, " ", "[]");
	if(data[0] == '[')
	{
		Log("Found command...");
		if(toks[1] == "AddClient")
		{
			Log("Calling 'AddClient'");
		//	TriggerEvent("AddClient", toks[2], toks[3]);
			/// @todo Adapt for any command to be registered with ConnectEvent()
		//	OnAddClient(toks[2], toks[3]);
		}
	}

	// Receive normal message
	TriggerEvent("ReceiveMessage", sender, data);
	Log("Received message: '"+data+"' from '"+sender+"'.\n");
	SendData(data, sender);
	messages.push_back(data);
}

void ChatServer::OnAccept(const boost::system::error_code &error)
{
	Server::OnAccept(error);
	OnAddClient("client", "");
}

void ChatServer::Accept(asio::ip::tcp::socket &sock)
{
	acceptor.async_accept(sock, boost::bind(&Server::OnAccept, this, asio::placeholders::error));
}

void ChatServer::OnAddClient(string name, string ip)
{
	Log("Adding client...");
	ChatClient* cc = new ChatClient(*io);
	cc->name = name;
	cc->host = ip;
	clients.push_back(cc);
	Accept(cc->socket);
}

void ChatServer::SendData(string msg)
{
	SendData(msg, name);
}

void ChatServer::SendData(string msg, string sender)
{
	if(msg[0] == '/')
	{
		// Parse as command
	}

	msg = sender+":"+msg;
	Server::SendData(msg);
	for(uint i = 0; i < clients.size(); ++i)
	{
		if(clients[i]->name == sender) continue;
		Server::SendData(msg, clients[i]->socket);
	}
	messages.push_back(msg);
	Log("Message sent: '"+msg+"' to ", clients.size(), " clients.");

}

void ChatServer::GetData()
{
	Server::GetData(socket);
	for(uint i = 0; i < clients.size(); ++i)
	{
		Server::GetData(clients[i]->socket);
	}
}

}
