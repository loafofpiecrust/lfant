
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/net/User.h>
#include <lfant/net/tcp/Connection.h>

// External
#include <boost/asio.hpp>
#include <array>


namespace lfant {
namespace net {
namespace tcp {

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
//	void Load(Properties* prop);

	// Main Loop Functions
	virtual void Init();
	virtual void Deinit();

	// Networking Functions
	virtual void Connect(string host = "127.0.0.1", uint16 port = 22222, string password = "");
	virtual void Disconnect(string ip);

	// Asio Callbacks
	virtual void OnConnect(const boost::system::error_code& error, net::Connection* con);

//	asio::ip::tcp::acceptor acceptor;
//	asio::ip::tcp::socket socket;
	asio::ip::tcp::resolver resolver;
	asio::ip::tcp::resolver::iterator iterator;
	asio::ip::tcp::endpoint endpoint;

private:

	string lastType = "";
	string lastPath = "";
//	uint32 peers = 0;
};

}
}
}
