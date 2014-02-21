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
	std::deque<string> messages;
};

}
