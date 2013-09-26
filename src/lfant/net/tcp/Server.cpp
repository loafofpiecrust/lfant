
#include <lfant/net/tcp/Server.h>

// Internal
#include <lfant/Console.h>

// External

namespace lfant {
namespace net {
namespace tcp {

IMPLEMENT_TYPE(net::User, net::tcp::Server)

Server::Server() /*:
	acceptor(*io)*/
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
	acceptor = new asio::ip::tcp::acceptor(*io, endpoint);
	acceptor->set_option(asio::socket_base::reuse_address(true));
/*	acceptor.open(endpoint.protocol());
	acceptor.bind(endpoint);
	acceptor.listen();*/
	started = true;
	Log("Server::Accept: Calling acceptor.async_accept().");

	Accept();
}

void Server::Accept()
{
	auto con = AddConnection<net::tcp::Connection>();
	Log("Accepting by new connection: ", con);
	Accept(con);
}

void Server::Accept(net::tcp::Connection* con)
{
	acceptor->async_accept(con->socket, boost::bind(&Server::OnAccept, this, asio::placeholders::error, (net::Connection*)con));
}

void Server::OnAccept(const boost::system::error_code& error, net::Connection* con)
{
	if(error)
	{
		Log("Server::OnAccept: Error.");
		Log(error.message());
		started = false;
	}

	Log("Server::OnAccept: Server 'accepted'(?), do something with it.");
	con->GetData();
//	Accept(con);
	Accept();
}

} // tcp
} // network
} // lfant
