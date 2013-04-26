/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
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
#pragma once

#include <lfant/stdafx.h>

// External
#include <boost/asio.hpp>
#include <forward_list>

// Internal
#include <lfant/ptr.h>
#include <lfant/Subsystem.h>

namespace lfant
{

namespace net {
class Connection;
}

/**	@addtogroup Game
 *	@{
 */
/**	@addtogroup Networking
 *	@{
 */

/**	This class manages Networking capabilities
 *		Uses RakNet for networking, with a p2p basis, but also with
 *		the ability to use FCM. Setup for a game server and chat server,
 *		as separate peer instances. Custom
 */
class Network : public Subsystem
{
	friend class Game;

public:
	Network();
	virtual ~Network();

	virtual void Init();
	virtual void Update();

	void Save(Properties *prop);
	void Load(Properties *prop);

	template<typename C>
	C* AddConnection(string name = "")
	{
		C* con = new C;
		connections.push_back(con);
		return con;
	}

	net::Connection* GetConnection(string name);

protected:
//	byte GetPacketId(RakNet::Packet* p);

public:
	deque< ptr<net::Connection> > connections;

};

/** @} */
/** @} */
}
