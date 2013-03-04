
#pragma once
#include <lfant/stdafx.h>

// External
#include <boost/asio.hpp>

// Internal
#include <lfant/Object.h>

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

protected:
	Connection(string name, string host, uint16_t port, string password);

	virtual void Destroy();

	string name = "";
	string host = "";
	uint16_t port = 80;
	string password = "";

	boost::asio::io_service io;

	bool started = false;
};

}
