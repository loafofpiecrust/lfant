
#include <lfant/net/udp/Connection.h>

// External

// Internal
#include <lfant/Console.h>

namespace lfant {
namespace net {
namespace udp {

Connection::Connection() :
	socket(*io)
{
}

Connection::Connection(asio::io_service &io) :
	net::Connection(io),
	socket(io)
{
}

Connection::~Connection()
{
}

void Connection::OnDestroy()
{
	io->stop();
}

void Connection::OnGetData(const boost::system::error_code &error)
{
	Log("tcp::Connection::OnGetData: Touch.");
	if(error)
	{
		Log("Connection::OnGetData: Error '"+error.message()+"'.");
	//	return;
	}

	GetData();
}

void Connection::OnSendData(const boost::system::error_code &error, size_t bytes)
{
	Log("Connection::OnSendData: Touch.");
	if(error)
	{
		Log("Connection::OnSendData: Error '"+error.message()+"'.");
		return;
	}
	else
	{
	//	this->OnSendData(error, bytes);
	}
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
