
#include <lfant/net/Connection.h>

// internal
#include <lfant/Console.h>

// external

namespace lfant {
namespace net {

void Connection::OnSendData(const boost::system::error_code &error, size_t bytes)
{
	Log("Connection::OnSendData: Touch.");
	if(error)
	{
		Log("Connection::OnSendData: Error '"+error.message()+"'.");
		return;
	}
	else
	{
	//	this->OnSendData(error, bytes);
	}
}

void Connection::OnGetData(const boost::system::error_code &error)
{
	Log("tcp::Connection::OnGetData: Touch.");
	if(error)
	{
		Log("Connection::OnGetData: Error '"+error.message()+"'.");
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