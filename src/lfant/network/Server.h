
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/network/Connection.h>

// External

namespace lfant
{
namespace network
{

class Server : public Connection
{
public:
	Server();
	virtual ~Server();

	void Init();
	void Update();

	void Host();
	void OnAccept(const boost::system::error_code &error);
	void OnSendData(const boost::system::error_code &error, size_t bytes);
	void OnGetData(const boost::system::error_code &error);

	void SetPort(uint16 port);

	void SendData(string data);

	string GetData(uint32 size);

protected:

private:
	asio::ip::tcp::acceptor acceptor;
	asio::ip::tcp::socket socket;

	char lastData[64];

};

}
}
