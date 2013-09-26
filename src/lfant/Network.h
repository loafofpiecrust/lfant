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

// Internal
#include <lfant/ptr.h>
#include <lfant/Subsystem.h>
#include <lfant/Component.h>

// External
#include <boost/asio.hpp>
#include <forward_list>
#include <type_traits>

namespace lfant
{

namespace net {
class User;
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

	virtual void Save(Properties *prop) const;
	virtual void Load(Properties *prop);

	template<typename C>
	C* AddUser()
	{
		C* usr = new C;
		users.push_back(usr);
		return usr;
	}

	net::User* AddUser(string type);

	net::User* GetUser(string name) const;

protected:
//	byte GetPacketId(RakNet::Packet* p);

public:
	deque< ptr<net::User> > users;

private:

};

/** @} */
/** @} */
}
