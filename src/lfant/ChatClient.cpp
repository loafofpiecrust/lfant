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

void ChatClient::SendData(string msg)
{
	if(msg[0] == '/')
	{
		// Parse as command
	}

	msg = name+":"+msg;
	Client::SendData(msg);
	messages.push_back(msg);
	Log("Message sent: '"+msg+"'.");
}

void ChatClient::Disconnect()
{
	SendData("[RemoveClient "+name+"]");
	Client::Disconnect();
}

void ChatClient::OnDestroy()
{
	Disconnect();
}

void ChatClient::OnConnect(const boost::system::error_code &error)
{
	Client::OnConnect(error);
	Log("ChatClient::OnConnect: Success.");
	SendData("[AddClient "+name+" "+socket.remote_endpoint().address().to_string()+"]");
//	GetData();
}

void ChatClient::OnGetData(const boost::system::error_code &error)
{
	Client::OnGetData(error);

	string data = lastData;

	// Receive normal message
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
	TriggerEvent("ReceiveMessage", sender, data);
	Log("Received message: '"+data+"' from '"+sender+"'.\n");
	messages.push_back(data);
}

void ChatClient::OnSendData(const boost::system::error_code &error, size_t bytes)
{
	Log("ChatClient::OnSendData: Touch.");
	Client::OnSendData(error, bytes);
}

}
