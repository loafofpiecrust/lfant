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
#include "stdafx.hpp"

// External
#include <RakPeerInterface.h>
#include <FullyConnectedMesh2.h>
#include <ConnectionGraph2.h>
#include <RakNetTypes.h>

// Internal
#include "Subsystem.hpp"

namespace sfs
{
/**	@addtogroup Engine
 *	@{
 */
/**	@addtogroup Subsystems
 *	@{
 */

/** Each instance of Connection handles a connection to a server.
 *	@details
 *		Also, extension instances are included. For example, I
 *		include FullyConnectedMesh2 and ConnectionGraph2 for new
 *		kinds of connections.
 *	@todo
 *		If possible, make it more interchangeable.
 */
class Connection
{
public:
	RakNet::RakPeerInterface* peer;
	RakNet::FullyConnectedMesh2 fcm;
	RakNet::ConnectionGraph2 cg;
	RakNet::SocketDescriptor socket;
	uint16_t maxPeers = 64;

	Connection(uint16_t port, uint16_t maxPeers = 64) :
		socket(port, 0), maxPeers(maxPeers)
	{
		peer = RakNet::RakPeerInterface::GetInstance();
		peer->AttachPlugin(&fcm);
		peer->AttachPlugin(&cg);
		fcm.SetAutoparticipateConnections(true);
		// Attach any more plugins you want here.
	}

	~Connection()
	{
		RakNet::RakPeerInterface::DestroyInstance(peer);
	}

	bool ready = false;
};

/**	This class manages Networking capabilities
 *	@details
 *		Uses RakNet for networking, with a p2p basis, but also with
 *		the ability to use FCM. Setup for a game server and chat server,
 *		as separate peer instances. Custom
 *	@todo
 *		Todo
 */
class Network: public Subsystem
{
	friend class Engine;

public:
	Network();
	virtual ~Network();

	/**	Attempts to host a sole server via IPv6 and/or IPv4
	 *	@details
	 *		Hosts a server that allows direct connection, but not FCM. If
	 *		FCM2 capabilities are desired, \p ConnectFCM will "host" a server
	 *		if there isn't one on the given IP.
	 *	@todo
	 *		Admin controls when host.
	 *		Differentiate between client and host (commands, and scoreboard).
	 */
	bool Setup(Connection* con);

	/**	Connects client to a remote host via IP
	 *	@details
	 *		Connects the client to a server directly, without
	 *		host switching. This type of connection is inflexible
	 *		and depends on the host server.
	 */
	bool Connect(Connection* con, string host, string password = "");

	/**	Connects clients via FCM2.
	 *	@details
	 *		FullyConnectedMesh2 Connects a group of clients together.
	 *		The first to "connect", before the rest of the clients connect,
	 *		becomes the host. Then, if he disconnects, the next best candidate
	 *		becomes host.
	 *	@todo
	 *		Have a specifier enum to turn off admin controls in FCM mode.
	 */
	bool ConnectFCM(Connection* con, string host, string password = "");

	/**	Disconnects from the current server.
	 *	@details
	 *		Disconnects the client from the server it is currently connected to,
	 *		whether it is FCM, Direct, or chat.
	 *	@todo
	 *		Have different ports for different connection types, so specific connections can be ended.
	 *		Grab those ports from XML, or config.
	 */
	void Disconnect(Connection* con);

protected:
	virtual void Init();
	virtual void Update();

public:
	Connection* chatPeer;
	Connection* gamePeer;
};

/** @} */
/** @} */
}
