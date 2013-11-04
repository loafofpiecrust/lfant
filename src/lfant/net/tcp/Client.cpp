
#include <lfant/net/tcp/Client.h>

// External

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>

namespace lfant {
namespace net {
namespace tcp {

Client::Client():
	resolver(*io)
{
}

Client::Client(asio::io_service &io) :
	User(io),
	resolver(io)
{
}

Client::~Client()
{
}

void Client::Init()
{
}

void Client::Deinit()
{
	User::Deinit();
}

void Client::Connect(string host, uint16 port, string password)
{
	auto con = AddConnection<net::tcp::Connection>();
	con->remoteIp = host;
	con->port = port;

	boost::asio::ip::tcp::resolver::query query(host, lexical_cast<string>(port));
	iterator = resolver.resolve(query);
//	endpoint = *iterator;
//	socket.async_connect(endpoint, boost::bind(&Client::OnConnect, this, boost::asio::placeholders::error));
	boost::asio::async_connect(con->socket, iterator, boost::bind(&Client::OnConnect, this, boost::asio::placeholders::error, (net::Connection*)con));
	started = true;
}

void Client::Disconnect(string ip)
{
	for(uint i = 0; i < connections.size(); ++i)
	{
		if(connections[i]->remoteIp == ip)
		{
			connections[i]->Deinit();
			connections.erase(connections.begin()+i);
			break;
		}
	}
	if(connections.size() <= 0)
	{
		started = false;
	}
}

/*
void Client::SendData(Properties *prop, string path)
{
	fstream stream("temp/data.prop");
	prop->SaveStream(stream);

	string line = "";
	SendData(":/Properties");
	while(stream.good())
	{
		getline(stream, line);
		SendData(line);
	}
	SendData(":\\Properties");
	stream.close();

	remove("temp/data.prop");
}
*/

void Client::OnConnect(const boost::system::error_code& error, net::Connection* con)
{
	if (error)
	{
		Log(error.message());
		return;
	}
	TriggerEvent("Connect", error.message());
	con->GetData();
}

}
}
}
