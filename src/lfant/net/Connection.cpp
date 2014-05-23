
#include <lfant/net/Connection.h>

// internal
#include <lfant/Console.h>

// external

namespace lfant {
namespace net {

void Connection::OnSendData(const boost::system::error_code &error, size_t bytes)
{
	std::cout<<("Connection::OnSendData: Touch.\n");
	if(error)
	{
		std::cout<<("Connection::OnSendData: Error '"+error.message()+"'.\n");
		return;
	}
	else
	{
	//	this->OnSendData(error, bytes);
	}
}

void Connection::OnGetData(const boost::system::error_code &error)
{
	std::cout<<("tcp::Connection::OnGetData: Touch.\n");
	if(error)
	{
		std::cout<<("Connection::OnGetData: Error '"+error.message()+"'.\n");
		if(error.message() == "End of file")
		{
			TriggerEvent("Remove", this);
			return;
		}
	}

	TriggerEvent("GetData", string(lastData), (net::Connection*)this);
	GetData();
}

}
}
