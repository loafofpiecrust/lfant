/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-19 by Taylor Snead
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

#include <lfant/Network.hpp>

// External

// Internal

#include <lfant/Console.hpp>
#include <lfant/Thread.hpp>

#include <lfant/network/Connection.hpp>

namespace lfant
{

Network::Network()
{
	//Init();
}

Network::~Network()
{
}

void Network::Init()
{
	Log("Network::Init: Touch.");
}

void Network::Update()
{
	for(auto& con : connections)
	{
		con->Update();
	}
}

Connection* Network::GetConnection(string name)
{
	for(auto& con : connections)
	{
		if(con->name == name)
		{
			return con.get();
		}
	}
	return nullptr;
}

}

