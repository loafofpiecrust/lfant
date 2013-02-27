/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

#include <lfant/ChatClient.hpp>

// External

// Internal
#include <lfant/network/Client.hpp>
#include <lfant/Network.hpp>
#include <lfant/Console.hpp>

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
	connection = game->network->NewConnection<Client>("Chat", "www.google.com", 80, "");
	Connect(SENDER(connection, Connect), RECEIVER(this, OnConnect));
	Connect(SENDER(connection, GetData), RECEIVER(this, OnGetData));
}

void ChatClient::OnConnect()
{
	Log("Connected! 2");
	connection->GetData(2);
}

void ChatClient::OnGetData(char data[], uint size)
{
	Log("ChatClient::OnGetData: Received data:\n\t", data, "\n");
}

}
