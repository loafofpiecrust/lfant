
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/Object.h>

// external
#include <boost/asio.hpp>

namespace lfant {
namespace net {

class Connection : public lfant::Object
{
public:
	virtual ~Connection() {}

	virtual void GetData() {}
	virtual void SendData(string data) {}
	virtual void Deinit() {}

	virtual void OpenSocket() {}

	string remoteIp = "127.0.0.1";
	uint16 port = 0;
	char lastData[256];

protected:
	virtual void OnSendData(const boost::system::error_code &error, size_t bytes);
	virtual void OnGetData(const boost::system::error_code &error);

private:

};

}
}