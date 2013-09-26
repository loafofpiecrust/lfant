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

// Internal
#include <lfant/Network.h>
#include <lfant/Console.h>
#include <lfant/SystemInfo.h>
#include <lfant/ChatServer.h>

// External

namespace lfant
{

IMPLEMENT_TYPE(net::User, ChatClient)

ChatClient::ChatClient()
{
	name = game->systemInfo->computerName;
}

ChatClient::ChatClient(asio::io_service& new_io) :
	net::tcp::Client(new_io)
{
}

ChatClient::~ChatClient()
{
}

void ChatClient::Init()
{
}

void ChatClient::SendMessage(string msg)
{
	if(msg[0] == '/')
	{
		// Parse as command
	}

	msg = name+":"+msg;
	for(auto& con : connections)
	{
		con->SendData(msg);
	}
	messages.push_back(msg);
	Log("Message sent: '"+msg+"'.");
}

void ChatClient::Disconnect(string ip)
{
	for(auto& con : connections)
	{
		con->SendData("[RemoveClient "+name+"]");
	}
	Client::Disconnect(ip);
}

void ChatClient::Deinit()
{
	Client::Deinit();
}

void ChatClient::OnConnect(const boost::system::error_code &error, net::Connection* con)
{
	if(error) return;
	Client::OnConnect(error, con);
	
	con->SendData("Yola");

	Log("ChatClient::OnConnect: Success.");
}

void ChatClient::OnGetData(string data, net::Connection* con)
{
	Client::OnGetData(data, con);
	TriggerEvent("ReceiveMessage", string(""), data);
}

}
