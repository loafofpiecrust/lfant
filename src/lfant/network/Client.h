
#pragma once
#include <lfant/stdafx.h>

// External
#include <boost/asio.hpp>

// Internal
#include <lfant/network/Connection.h>

namespace lfant
{

class Client : public Connection
{
public:
	Client(string name, string host, uint16_t port, string password);
	virtual ~Client();

	// Main Loop Functions
	virtual void Init();
	virtual void Update();
	virtual void Destroy();

	// Networking Functions
	void Connect();
	void Disconnect();

	void GetData(uint size);

	// Asio Callbacks
	void OnConnect(const boost::system::error_code& error);
	void OnGetData(const boost::system::error_code& error);

	boost::asio::ip::tcp::socket socket {io};
	boost::asio::ip::tcp::resolver resolver {io};
	boost::asio::ip::tcp::resolver::iterator iterator;
	boost::asio::ip::tcp::endpoint endpoint;

	bool connected = false;

private:
	char lastData[516];

};

}
