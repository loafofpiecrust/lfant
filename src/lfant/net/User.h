
#pragma once
#include <lfant/stdafx.h>

// Internal
#include <lfant/TypeRegistry.h>
#include <lfant/Object.h>
#include <lfant/net/Connection.h>

// External
#include <boost/asio/io_service.hpp>
#include <array>

namespace asio = boost::asio;

namespace lfant {
namespace net {

class User : public Object
{
	DECLARE_REGISTRY(net::User)

public:
	virtual ~User();

	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	virtual void OnAddConnection(net::Connection* con);

	template<typename C>
	C* AddConnection()
	{
		C* con = new C(*io);
		connections.push_back(con);
		OnAddConnection(con);
		return con;
	//	con->GetData();
	}

	void RemoveConnection(net::Connection* con);

	virtual uint32 GetConnectionCount() { return connections.size(); }


	string name = "";

protected:
	User();
	User(asio::io_service& io);

	virtual void OnGetData(string data, net::Connection* con);

	bool started = false;
	std::shared_ptr<asio::io_service> io {new asio::io_service};
	std::deque<ptr<net::Connection>> connections;

private:
};

}
}
