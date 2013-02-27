
#include "Connection.hpp"

// External

// Internal

namespace lfant
{

Connection::Connection(string name, string host, uint16_t port, string password) :
	name(name), host(host), port(port), password(password)
{
}

void Connection::Destroy()
{
	Object::Destroy();
}

}
