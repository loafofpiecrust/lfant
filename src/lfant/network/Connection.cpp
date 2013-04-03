
#include <lfant/network/Connection.h>

// External

// Internal

namespace lfant
{

Connection::Connection(string name, string host, uint16 port, string password) :
	name(name), host(host), port(port), password(password)
{
}

void Connection::Destroy()
{
	Object::Destroy();
}

void Connection::SetPort(uint16 port)
{
	this->port = port;
}

uint16 Connection::GetPort()
{
	return port;
}

}
