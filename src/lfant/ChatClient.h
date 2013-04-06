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

#include <lfant/Component.h>

namespace lfant
{

namespace network
{
class Client;
}

struct Peer
{
	string name = "lfantClient";
	string ip = "127.0.0.1";
};

/**
 *
 */
class ChatClient : public Component
{
public:
	ChatClient();
	virtual ~ChatClient();

	virtual void Init();
	void Update();

	void SendMessage(string msg);
	void Host();
	void Connect();
	void Disconnect();

protected:

	void ReceiveMessage(string msg);

	void OnDestroy();

	// Callbacks
	void OnConnect(string error = "");
	void OnDisconnect(string error = "");
	void OnHost(string error = "");
	void OnGetData(string data);

	network::Client* client;
	string lastMsg;
	string name = "lfantClient";
	deque<Peer> peers;

private:
};

}
