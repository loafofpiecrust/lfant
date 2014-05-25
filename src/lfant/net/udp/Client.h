
#pragma once
#include <lfant/stdafx.h>

// External
#include <boost/asio.hpp>
#include <array>

// Internal
#include <lfant/TypeRegistry.h>
#include <lfant/net/udp/Connection.h>
#include <lfant/net/User.h>

namespace lfant {
namespace net {
namespace udp {

/**
 *	Allows broadcasting, multicasting, and onecasting.
 */
class Client : public net::User
{
public:
	Client();
	Client(asio::io_service& io);
	virtual ~Client();

//	void Save(Properties* prop);
//	void Serialize(Properties* prop);

	// Main Loop Functions
	virtual void Init();
	virtual void Deinit();

	// Networking Functions
	void Connect(string host = "127.0.0.1", uint16 port = 22222, string password = "");
	void Disconnect(string ip);

	// Asio Callbacks
	virtual void OnConnect(const boost::system::error_code& error, net::Connection* con);

//	asio::ip::tcp::acceptor acceptor;
//	asio::ip::tcp::socket socket;
	asio::ip::udp::resolver resolver;
	asio::ip::udp::resolver::iterator iterator;
	asio::ip::udp::endpoint endpoint;

private:

	string lastType = "";
	string lastPath = "";
//	uint32 peers = 0;
};

}
}
}
