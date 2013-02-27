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
#pragma once

#include <lfant/stdafx.hpp>

// External
#include <boost/asio.hpp>
#include <forward_list>

// Internal

#include <lfant/Subsystem.hpp>

namespace lfant
{

class Connection;

/**	@addtogroup Engine
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
	friend class Engine;

public:
	Network();
	virtual ~Network();

	virtual void Init();
	virtual void Update();

	template<typename C>
	C* NewConnection(string name, string host, uint16_t port, string password)
	{
		C* con = new C(name, host, port, password);
		connections.push_front(unique_ptr<Connection>(con));
		con->Init();
		return con;
	}

	Connection* GetConnection(string name);

protected:
//	byte GetPacketId(RakNet::Packet* p);

public:
	forward_list< unique_ptr<Connection> > connections;

};

/** @} */
/** @} */
}
