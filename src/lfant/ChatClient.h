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
#include <lfant/TypeRegistry.h>
#include <lfant/net/tcp/Client.h>

namespace lfant
{

/**
 *
 */
class ChatClient : public net::tcp::Client
{
	DECLARE_TYPE(net::User, ChatClient)
	
public:
	ChatClient();
	ChatClient(asio::io_service& new_io);
	virtual ~ChatClient();

	virtual void Init();

	void SendMessage(string msg);
	void Disconnect(string ip = "127.0.0.1");

protected:

	void Deinit();

	// Callbacks
	virtual void OnConnect(const boost::system::error_code& error, net::Connection* con);
	void OnDisconnect(const boost::system::error_code& error);
	virtual void OnGetData(string data, net::Connection* con);

	deque<string> messages;

private:
};

}
