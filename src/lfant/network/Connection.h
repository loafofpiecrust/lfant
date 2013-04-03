
#pragma once
#include <lfant/stdafx.h>

// External
#include <boost/asio.hpp>

// Internal
#include <lfant/Object.h>

namespace asio = boost::asio;

namespace lfant
{

/** Each instance of Connection handles a connection to a server.
 *		Also, extension instances are included.
 *	@todo
 *		If possible, make it more interchangeable.
 */
class Connection : public Object
{
	friend class Network;

public:

	virtual ~Connection()
	{
	}

	void SetPort(uint16 port);
	uint16 GetPort();

protected:
	Connection(string name, string host, uint16 port, string password);
	Connection() {}

	virtual void Destroy();

	string name = "";
	string host = "127.0.0.1";
	uint16 port = 0;
	string password = "";

	asio::io_service io;

	bool started = false;
};

}
