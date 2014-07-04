
#include <lfant/net/tcp/Connection.h>

// External

// Internal
#include <lfant/Console.h>
#include <lfant/Game.h>

namespace lfant {
namespace net {
namespace tcp {

Connection::Connection(asio::io_service &io) :
	socket(io)
{
}

Connection::~Connection()
{
}

void Connection::OpenSocket()
{
	socket.open(boost::asio::ip::tcp::v4());
}

void Connection::Deinit()
{
	boost::system::error_code ec;
	socket.shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
	socket.close( ec );
}

void Connection::GetData()
{
	boost::asio::async_read(socket, boost::asio::buffer(lastData, 256),
							boost::bind(&Connection::OnGetData, this, boost::asio::placeholders::error)
							);
}

void Connection::SendData(string data)
{
	DoSendData(data);
}

void Connection::DoSendData(string data)
{
	uint i = 0;
	for(; i < data.size(); ++i)
	{
		lastData[i] = data[i];
	}
	for(; i < 256; ++i)
	{
		lastData[i] = '\0';
	}

	GetGame()->Log("Connection::SendData: About to write data.");
	asio::async_write(socket, boost::asio::buffer(lastData, 256), boost::bind(&Connection::OnSendData, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

void Connection::OnGetData(const boost::system::error_code &error)
{
	net::Connection::OnGetData(error);
}

}
}
}
