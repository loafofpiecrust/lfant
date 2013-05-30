
#include <lfant/net/tcp/Server.h>

// Internal
#include <lfant/Console.h>

// External

namespace lfant {
namespace net {
namespace tcp {

Server::Server() :
	acceptor(*io)
{
}

Server::~Server()
{
}

void Server::Init()
{
}

void Server::Host(uint16 port, string password)
{
	asio::ip::tcp::endpoint endpoint (asio::ip::address::from_string("127.0.0.1"), port);
	acceptor.open(endpoint.protocol());
	acceptor.set_option(asio::socket_base::reuse_address(true));
	acceptor.bind(endpoint);
	acceptor.listen();
	Log("Server::Accept: Calling acceptor.async_accept().");
	acceptor.async_accept(socket, boost::bind(&Server::OnAccept, this, asio::placeholders::error));
	started = true;
}

void Server::OnAccept(const boost::system::error_code& error)
{
	if(error)
	{
		Log("Server::OnAccept: Error '", error.message(), "'.");
	}

	Log("Server::OnAccept: Server 'accepted'(?), do something with it.");
	GetData();
}

} // tcp
} // network
} // lfant
