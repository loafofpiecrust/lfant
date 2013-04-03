
#pragma once
#include <lfant/stdafx.h>

// External
#include <boost/asio.hpp>
#include <array>

// Internal
#include <lfant/network/Connection.h>

namespace lfant
{
namespace network
{

class Client : public Connection
{
public:
	Client();
	virtual ~Client();

//	void Save(Properties* prop);
//	void Load(Properties* prop);

	// Main Loop Functions
	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	// Networking Functions
	void Connect(string host = "127.0.0.1", uint16 port = 22222);
	void Disconnect();

	void Host(uint16 port = 22222);

	void GetDataAsync(uint32 size);
	string GetData(uint32 size);

	void SendData(string data);
	void SendData(Properties* prop, string path);

	void SendFile(string path, string outpath = "");

	// Asio Callbacks
	void OnConnect(const boost::system::error_code& error);
	void OnAccept(const boost::system::error_code& error);
	void OnGetData(const boost::system::error_code& error);
	void OnSendData(const boost::system::error_code& error, size_t bytes);

	asio::ip::tcp::acceptor acceptor;
	asio::ip::tcp::socket socket;
	asio::ip::tcp::resolver resolver;
	asio::ip::tcp::resolver::iterator iterator;
	asio::ip::tcp::endpoint endpoint;

	bool connected = false;

private:
	void Accept();

	char lastData[256];
	uint32 lastSize;
	string lastType = "";
	string lastPath = "";
	uint32 peers = 0;
};

}
}
