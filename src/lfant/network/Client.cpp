
#include <lfant/network/Client.h>

// External

// Internal
#include <lfant/Console.h>

namespace lfant
{

Client::Client(string name, string host, uint16_t port, string password) :
	Connection(name, host, port, password)
{
}

Client::~Client()
{
}

void Client::Init()
{
	Connect();
}

void Client::Update()
{
	if(connected)
	{
		io.poll();
	}
}

void Client::Destroy()
{
	io.stop();
	Connection::Destroy();
}

void Client::Connect()
{
	boost::asio::ip::tcp::resolver::query query(host, lexical_cast<string>(port));
	iterator = resolver.resolve(query);
	//	endpoint = *iterator;
	//	socket.async_connect(endpoint, boost::bind(&Client::OnConnect, this, boost::asio::placeholders::error));
	boost::asio::async_connect(socket, iterator, boost::bind(&Client::OnConnect, this, boost::asio::placeholders::error));
	connected = true;
}

void Client::Disconnect()
{
	//	boost::system::error_code ec;
	//	sock.shutdown( boost::asio::ip::tcp::socket::shutdown_both, ec );
	//	sock.close( ec );
	connected = false;
}

void Client::GetData(uint size)
{
	Log("Client::GetData: Touch.");
//	lastSize = size;
	//boost::asio::read(socket, boost::asio::buffer(&lastData[0], lastSize));
	boost::asio::async_read(socket, boost::asio::buffer(lastData, 4),
	                        boost::bind(&Client::OnGetData, this, boost::asio::placeholders::error)
	                        );
}

void Client::OnConnect(const boost::system::error_code& error)
{
	if (!error)
	{
		Log("Connected!");
		Trigger("Connect");
	}
	else
	{
		Log(error.message());
		Disconnect();
	}
}

void Client::OnGetData(const boost::system::error_code& error)
{
	Log("Client::OnGetData: Touch.");
	if(!error)
	{
		Log("Getting data");
		Trigger("GetData", lastData, 4);
	}
	else
	{
		Log(error.message());
		Disconnect();
	}
}

}
