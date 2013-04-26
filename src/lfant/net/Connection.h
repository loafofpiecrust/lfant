
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/Object.h>

// External
#include <boost/asio/io_service.hpp>
#include <array>

namespace asio = boost::asio;

namespace lfant {
namespace net {

class Connection : public Object
{
public:
	virtual ~Connection();

	void Update();

	virtual void GetData() = 0;
	virtual void SendData(string data) = 0;

	virtual void OnGetData(const boost::system::error_code& error) = 0;
	virtual void OnSendData(const boost::system::error_code& error, size_t bytes) = 0;

	uint16 GetPort();

	string name = "";
	string host = "127.0.0.1";

protected:
	Connection();
	Connection(asio::io_service& io);

	uint16 port = 0;
	bool started = false;
	shared_ptr<asio::io_service> io {new asio::io_service};

	char lastData[256];

private:
};

}
}
