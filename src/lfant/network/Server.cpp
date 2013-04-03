
#include <lfant/network/Server.h>

// Internal
#include <lfant/Console.h>

// External

namespace lfant
{
namespace network
{

Server::Server() :
	acceptor(io),
	socket(io)
{
}

Server::~Server()
{
}

void Server::Init()
{
	Log("Server::Init: Calling this->Accept().");
	if(port == 0)
	{
		SetPort(22222);
	}
	Host();
}

void Server::Update()
{
	io.poll();
}

void Server::Host()
{
	Log("Server::Accept: Calling acceptor.async_accept().");
	acceptor.async_accept(socket, boost::bind(&Server::OnAccept, this, asio::placeholders::error));
}

void Server::OnAccept(const boost::system::error_code& error)
{
	if(error)
	{
		Log("Server::OnAccept: Error '", error.message(), "'.");
	}

	Log("Server::OnAccept: Client connected, do something with it.");
}

void Server::OnSendData(const boost::system::error_code &error, size_t bytes)
{
	if(!error)
	{
		Log("Server::OnWrite: Sent ", bytes, " bytes of data.");
	}
	else
	{
		Log("Server::OnWrite: Error '", error.message(), "'.");
	}
}

void Server::OnGetData(const boost::system::error_code &error)
{
	Log("Client::OnGetData: Touch.");
	if(!error)
	{
		Log("Client::OnGetData: Received ", string(lastData).size(), " bytes of data.");
		TriggerEvent("GetData", string(lastData));
	}
	else
	{
		Log(error.message());
	//	Disconnect();
	}
}

void Server::SetPort(uint16 port)
{
	asio::ip::tcp::endpoint endpoint (asio::ip::address::from_string("127.0.0.1"), port);
	acceptor.open(endpoint.protocol());
	acceptor.set_option(asio::socket_base::reuse_address(true));
	acceptor.bind(endpoint);
	acceptor.listen();

	Connection::SetPort(port);
}

void Server::SendData(string data)
{
	asio::async_write(socket, asio::buffer(data), boost::bind(&Server::OnSendData, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

string Server::GetData(uint32 size)
{
	boost::asio::read(socket, asio::buffer(lastData, size));
	return lastData;
}

}
}
