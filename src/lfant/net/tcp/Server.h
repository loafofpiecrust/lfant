
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/net/tcp/Connection.h>

// External

namespace lfant {
namespace net {
namespace tcp {

class Server : public Connection
{
public:
	Server();
	virtual ~Server();

	void Init();

	void Host(uint16 port = 22222, string password = "");
	void Finish();
	virtual void OnAccept(const boost::system::error_code &error);

protected:
	asio::ip::tcp::acceptor acceptor;
	string password = "";

private:
//	asio::ip::tcp::socket socket;


};

}
}
}
