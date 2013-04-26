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
#include <lfant/net/tcp/Client.h>

namespace lfant
{

struct Peer;

/**
 *
 */
class ChatClient : public net::tcp::Client
{
public:
	ChatClient();
	ChatClient(asio::io_service& new_io);
	virtual ~ChatClient();

	virtual void Init();

	void SendData(string msg);
	void Disconnect();

protected:

	void OnDestroy();

	// Callbacks
	virtual void OnConnect(const boost::system::error_code& error);
	void OnDisconnect(const boost::system::error_code& error);
	void OnGetData(const boost::system::error_code& error);
	void OnSendData(const boost::system::error_code &error, size_t bytes);

//	string name = "lfantClient";
	deque< ptr<Peer> > peers;
	deque<string> messages;

private:
};

}
