
#include <lfant/net/udp/Client.h>

// External

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>

namespace lfant {
namespace net {
namespace udp {

IMPLEMENT_TYPE(net::Connection, net::udp::Client)

Client::Client():
	resolver(*io)
{
}

Client::Client(asio::io_service &io) :
	Connection(io),
	resolver(io)
{
}

Client::~Client()
{
}

void Client::Init()
{
}

void Client::OnDestroy()
{
	Disconnect();
	Connection::OnDestroy();
}

void Client::Connect(string host, uint16 port, string password)
{
	boost::asio::ip::udp::resolver::query query(host, lexical_cast<string>(port));
	iterator = resolver.resolve(query);
//	endpoint = *iterator;
//	socket.async_connect(endpoint, boost::bind(&Client::OnConnect, this, boost::asio::placeholders::error));
	boost::asio::async_connect(socket, iterator, boost::bind(&Client::OnConnect, this, boost::asio::placeholders::error));
	started = true;
}

void Client::Disconnect()
{
//	boost::system::error_code ec;
//	sock.shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
//	sock.close( ec );
	io->stop();
	io->post(boost::bind(&asio::ip::udp::socket::close, &socket));
	started = false;
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

void Client::SendFile(string path, string outpath)
{
	if(outpath == "")
	{
		outpath = path;
	}
	path = game->fileSystem->GetGamePath(path).string();

	SendData(":/File "+outpath);
	Log("File data block started...");

	ifstream stream(path);
	string line = "";
	while(stream.good())
	{
		Log("Getting line in a sent file...");
		getline(stream, line);
		if(line != "")
		{
			SendData(line);
		}
	}

	SendData(":\\File");
}

void Client::OnConnect(const boost::system::error_code& error)
{
	if (error)
	{
		Log(error.message());
		return;
	}
	TriggerEvent("Connect", error.message());
	GetData();
}

}
}
}
