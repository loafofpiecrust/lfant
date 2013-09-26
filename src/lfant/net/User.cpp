
#include <lfant/net/User.h>

// Internal
#include <lfant/Console.h>

// External

namespace lfant {
namespace net {

IMPLEMENT_REGISTRY(net::User)

User::User()
{
}

User::User(asio::io_service &io) :
	io(&io)
{
}

User::~User()
{
}

void User::Init()
{
	Object::Init();
}

void User::Update()
{
	if(started)
	{
		io->poll();

	/*	for(uint i = 0; i < connections.size(); ++i)
		{
			connections[i]->GetData();
		}*/
	}
}

void User::Deinit()
{
	for(auto& con : connections)
	{
		con->Deinit();
	}
	io->stop();
}

void User::RemoveConnection(net::Connection* con)
{
	for(uint i = 0; i < connections.size(); ++i)
	{
		if(connections[i] == con)
		{
			con->Deinit();
			connections.erase(connections.begin()+i);
			break;
		}
	}
}

void User::OnAddConnection(net::Connection* con)
{
	ConnectEvent(SENDER(con, Remove), RECEIVER(this, RemoveConnection));
	ConnectEvent(SENDER(con, GetData), RECEIVER(this, OnGetData));
}

void User::OnGetData(string data, net::Connection* con)
{
}

}
}
