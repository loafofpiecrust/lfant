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
#pragma once

#include <lfant/stdafx.h>

// External

// Internal

#include <lfant/net/tcp/Server.h>

namespace lfant
{

class ChatClient;

struct Peer
{
	string name = "client";
	string ip = "127.0.0.1";
};

/**
 *
 */
class ChatServer : public net::tcp::Server
{
public:
	ChatServer();
	virtual ~ChatServer();

	virtual void Init();

//	void SendMessage(string msg);
	void SendData(string msg, string sender);
	void SendData(string msg);
	void GetData();

protected:
	virtual void OnGetData(const boost::system::error_code& error);
	virtual void OnGetData(const boost::system::error_code &error, asio::ip::tcp::socket* sock);
	void OnAccept(const boost::system::error_code &error);
	void Accept(asio::ip::tcp::socket& sock);
	void OnAddClient(string name, string ip);

private:
	deque< ptr<ChatClient> > clients;
	deque<string> messages;
};

}
