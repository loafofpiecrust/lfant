
#include <lfant/net/tcp/Connection.h>

// External

// Internal
#include <lfant/Console.h>

namespace lfant {
namespace net {
namespace tcp {

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
	OnGetData(error, &socket);
}

void Connection::OnGetData(const boost::system::error_code &error, asio::ip::tcp::socket* sock)
{
	Log("tcp::Connection::OnGetData: Touch.");
	if(error)
	{
		Log("Connection::OnGetData: Error '"+error.message()+"'.");
	//	return;
	}

	GetData(*sock);
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
	GetData(socket);
}

void Connection::GetData(asio::ip::tcp::socket &sock)
{
	boost::asio::async_read(sock, boost::asio::buffer(lastData, 256),
							boost::bind(&Connection::OnGetData, this, boost::asio::placeholders::error, &sock)
							);
}

void Connection::SendData(string data)
{
	SendData(data, socket);
}

void Connection::SendData(string data, asio::ip::tcp::socket &sock)
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
	asio::async_write(sock, asio::buffer(lastData), boost::bind(&tcp::Connection::OnSendData, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
}

}
}
}
