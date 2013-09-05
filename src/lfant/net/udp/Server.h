
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/TypeRegistry.h>
#include <lfant/net/udp/Connection.h>

// External

namespace lfant {
namespace net {
namespace udp {

class Server : public Connection
{
	DECLARE_TYPE(net::Connection, net::udp::Server)
public:
	Server();
	virtual ~Server();

	void Init();

	void Host(uint16 port = 22222, string password = "");
	virtual void OnAccept(const boost::system::error_code &error);

protected:
	asio::ip::udp::endpoint endpoint;
//	asio::ip::udp::acceptor acceptor;
	string password = "";

private:
//	asio::ip::tcp::socket socket;


};

}
}
}
