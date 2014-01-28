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
#include <lfant/net/tcp/Server.h>

namespace lfant {

/**
 *
 */
class ChatServer : public net::tcp::Server
{
	DECLARE_TYPE(net::User, ChatServer)
public:
	ChatServer();
	virtual ~ChatServer();

	virtual void Init();

	virtual void OnAddConnection(net::Connection* con);

	void SendMessage(string msg);

protected:
	void OnAccept(const boost::system::error_code &error, net::Connection* con);
	void Accept();
	
private:
	deque<string> messages;
};

}
