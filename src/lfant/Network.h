/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
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
#include <mutex>

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
	Network(Game* game);
	virtual ~Network();

	virtual void Init();
	virtual void Update();

	virtual void Save(Properties *prop) const;
	virtual void Serialize(Properties *prop);

	template<typename C>
	C* AddUser()
	{
		C* usr = new C;
		users.push_back(usr);
		return usr;
	}

	net::User* AddUser(string type);

	net::User* GetUser(string name) const;

	std::mutex mutex;
	std::deque<ptr<net::User>> users;
protected:

private:

	void RunLoop();

};

/** @} */
/** @} */
}
