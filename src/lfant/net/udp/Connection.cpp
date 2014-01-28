
#include <lfant/net/udp/Connection.h>

// External

// Internal
#include <lfant/Console.h>

namespace lfant {
namespace net {
namespace udp {

Connection::Connection(asio::io_service &io) :
	socket(io)
{
}

Connection::~Connection()
{
}

void Connection::Deinit()
{
	boost::system::error_code ec;
	socket.shutdown( boost::asio::ip::udp::socket::shutdown_both, ec );
	socket.close( ec );
}

void Connection::OnGetData(const boost::system::error_code &error)
{
	net::Connection::OnGetData(error);
}

void Connection::OnSendData(const boost::system::error_code &error, size_t bytes)
{
	net::Connection::OnSendData(error, bytes);
}

void Connection::GetData()
{
//	boost::asio::async_read(socket, boost::asio::buffer(lastData, 256),
//							boost::bind(&Connection::OnGetData, this, boost::asio::placeholders::error)
//							);
}

void Connection::SendData(string data)
{
	Log("Connection::SendData: Touch.");
	uint i = 0;
	for(; i < data.size(); ++i)
	{
		lastData[i] = data[i];
	}
	for(; i < 256; ++i)
	{
		lastData[i] = '\0';
	}

	Log("Connection::SendData: About to write the data.");
//	socket.async_send_to(asio::buffer(lastData), endpoint, boost::bind(&Connection::OnSendData, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

}
}
}
