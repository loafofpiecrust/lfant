
#include <lfant/network/Client.h>

// External

// Internal
#include <lfant/Console.h>
#include <lfant/FileSystem.h>

namespace lfant
{
namespace network
{

Client::Client():
	socket(io),
	resolver(io),
	acceptor(io)
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
	Disconnect();
	Connection::Destroy();
}

void Client::Connect(string host, uint16 port)
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
	io.stop();
	io.post(boost::bind(&asio::ip::tcp::socket::close, &socket));
	connected = false;
}

void Client::Accept()
{
	acceptor.async_accept(socket, boost::bind(&Client::OnAccept, this, asio::placeholders::error));
}

void Client::Host(uint16 port)
{
	endpoint = asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), port);
	acceptor.open(endpoint.protocol());
	acceptor.set_option(asio::socket_base::reuse_address(true));
	acceptor.bind(endpoint);
	acceptor.listen();

	Accept();

	connected = true;

	TriggerEvent("Host");
}

void Client::GetDataAsync(uint32 size)
{
	Log("Client::GetData: Touch.");
	lastSize = size;
	//boost::asio::read(socket, boost::asio::buffer(&lastData[0], lastSize));
	boost::asio::async_read(socket, boost::asio::buffer(lastData, size),
							boost::bind(&Client::OnGetData, this, boost::asio::placeholders::error)
							);
}

string Client::GetData(uint32 size)
{
	boost::asio::read(socket, asio::buffer(lastData, size));
	return lastData;
}

void Client::SendData(string data)
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
	for(i = 0; i < peers; ++i)
	{
		asio::async_write(socket, asio::buffer(lastData), boost::bind(&Client::OnSendData, this, asio::placeholders::error, asio::placeholders::bytes_transferred));
	}
}

void Client::SendData(Properties *prop, string path)
{
	ofstream temp("temp/data.prop");
	prop->SaveStream(temp);
	ifstream stream("temp/data.prop");

	string line = "";
	SendData(":/Properties");
	while(stream.good())
	{
		getline(stream, line);
		SendData(line);
	}
	SendData(":\\Properties");
}

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
	if (!error)
	{
		Log("Connected!");
	}
	else
	{
		Log(error.message());
	//	Disconnect();
	}
	TriggerEvent("Connect", error.message());
}

void Client::OnAccept(const boost::system::error_code &error)
{
	if(error)
	{
		Log("Server::OnAccept: Error '", error.message(), "'.");
	}
	else
	{
		++peers;
	}

	Log("Server::OnAccept: Peer joined. Do something with this?");
	TriggerEvent("Accept", error.message());
//	Accept();
}

void Client::OnGetData(const boost::system::error_code& error)
{
	Log("Client::OnGetData: Touch.");
	if(!error)
	{
		string data = lastData;
		if(data.size() > 0)
		{
			if(data[0] == ':')
			{
				if(data[1] == '/')
				{
					// Begin structure
					data.erase(data.begin(), data.begin()+2);
					deque<string> toks = Split(data, " ");
					lastType = toks[0];
					Log("Client::OnGetData: Begin structure, "+lastType+".");
					if(lastType == "File" && toks.size() > 1)
					{
						lastPath = game->fileSystem->GetGamePath(toks[1]).string();
						ofstream stream(lastPath);
						stream.flush();
						stream << "\n";
						stream.close();
					}
				}
				else if(data[1] == '\\')
				{
					// End structure
					lastType = "";
					lastPath = "";
				}
			}
			else if(lastType != "")
			{
				// We're within a structure
				if(lastType == "Properties")
				{
					// We're being sent a Properties-generated file.
				}
				else if(lastType == "File")
				{
					// We're being sent a random file.
					ofstream stream(lastPath, ios_base::app);
					stream << data << "\n";
				}
			}
		}

	//	Log("Client::OnGetData: Received data, '", data, "'.");
		TriggerEvent("GetData", data);
	}
	else
	{
		Log("Client::OnGetData: Error '"+error.message()+"'.");
		//	Disconnect();
	}
	GetDataAsync(lastSize);
}

void Client::OnSendData(const boost::system::error_code &error, size_t bytes)
{
	if(!error)
	{
		Log("Server::OnSendData: Sent ", bytes, " bytes of data.");
	}
	else
	{
		Log("Server::OnSendData: Error '", error.message(), "'.");
	}
}

}
}
