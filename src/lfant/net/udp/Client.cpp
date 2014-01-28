
#include <lfant/net/udp/Client.h>

// External

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>

namespace lfant {
namespace net {
namespace udp {

IMPLEMENT_TYPE(net::User, net::udp::Client)

Client::Client():
	resolver(*io)
{
}

Client::Client(asio::io_service &io) :
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
	for(auto& con : connections)
	{
		con->Deinit();
	}
	User::Deinit();
}

void Client::Connect(string host, uint16 port, string password)
{
	auto con = AddConnection<net::udp::Connection>();
	con->remoteIp = host;
	con->port = port;

	boost::asio::ip::udp::resolver::query query(host, lexical_cast<string>(port));
	iterator = resolver.resolve(query);
//	endpoint = *iterator;
//	socket.async_connect(endpoint, boost::bind(&Client::OnConnect, this, boost::asio::placeholders::error));
	boost::asio::async_connect(con->socket, iterator, boost::bind(&Client::OnConnect, this, boost::asio::placeholders::error, con));
	started = true;
}

void Client::Disconnect(string ip)
{
	boost::system::error_code ec;
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
	fstream stream("temp/data.cfg");
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

	remove("temp/data.cfg");
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
