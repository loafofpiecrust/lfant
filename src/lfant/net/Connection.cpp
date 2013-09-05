
#include <lfant/net/Connection.h>

// Internal
#include <lfant/Console.h>

// External

namespace lfant {
namespace net {

IMPLEMENT_REGISTRY(net::Connection)

Connection::Connection()
{
}

Connection::Connection(asio::io_service &io) :
	io(&io)
{
}

Connection::~Connection()
{
}

void Connection::Update()
{
	if(started)
	{
		io->poll();
	}
}

uint16 Connection::GetPort()
{
	return port;
}

}
}
