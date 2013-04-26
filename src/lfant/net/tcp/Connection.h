
#pragma once
#include <lfant/stdafx.h>

// External
#include <boost/asio.hpp>
#include <array>

// Internal
#include <lfant/Object.h>
#include <lfant/net/Connection.h>

namespace asio = boost::asio;

namespace lfant {
namespace net {
namespace tcp {

/** Each instance of Connection handles a connection to a server.
 *		Also, extension instances are included.
 *	@todo
 *		If possible, make it more interchangeable.
 */
class Connection : public net::Connection
{
	friend class Network;
public:
	virtual void GetData();
	virtual void GetData(asio::ip::tcp::socket& sock);
	virtual void SendData(string data);
	virtual void SendData(string data, asio::ip::tcp::socket& sock);

	asio::ip::tcp::socket socket;

protected:
	Connection();
	Connection(asio::io_service& io);
	virtual ~Connection();

	void OnDestroy();

	virtual void OnGetData(const boost::system::error_code& error);
	virtual void OnGetData(const boost::system::error_code &error, asio::ip::tcp::socket *sock);
	virtual void OnSendData(const boost::system::error_code& error, size_t bytes);

};

}
}
}
