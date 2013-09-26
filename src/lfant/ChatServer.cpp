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
#include <lfant/net/tcp/Connection.h>

namespace lfant {

IMPLEMENT_TYPE(net::User, ChatServer)

ChatServer::ChatServer()
{
	name = game->systemInfo->computerName;
}

ChatServer::~ChatServer()
{

}

void ChatServer::Init()
{
//	ConnectEvent(SENDER(this, AddClient), RECEIVER(this, OnAddClient));
}

void ChatServer::OnAddConnection(net::Connection* con)
{
	ConnectEvent(SENDER(con, GetData), RECEIVER(this, SendMessage));
}

void ChatServer::OnAccept(const boost::system::error_code &error, net::Connection* con)
{
	Server::OnAccept(error, con);
}

void ChatServer::Accept()
{
	Server::Accept();
}

void ChatServer::SendMessage(string msg)
{
	TriggerEvent("ReceiveMessage", string(""), msg);
	if(msg[0] == '/')
	{
		// Parse as command
	}

//	msg = sender+":"+msg;
//	Server::SendData(msg);
	
	for(uint i = 0; i < connections.size(); ++i)
	{
	//	if(con->ip == sender) continue;
		connections[i]->SendData(msg);
	}
	
//	messages.push_back(msg);
//	Log("Message sent: '"+msg+"' to ", connections.size()-1, " clients.");
}

}
