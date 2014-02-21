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
#include <lfant/net/tcp/Client.h>

namespace lfant
{

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

	void SendMessage(string msg);
	void Disconnect(string ip = "127.0.0.1");

protected:

	void Deinit();

	// Callbacks
	virtual void OnConnect(const boost::system::error_code& error, net::Connection* con);
	void OnDisconnect(const boost::system::error_code& error);
	virtual void OnGetData(string data, net::Connection* con);

	std::deque<string> messages;

private:
};

}
