
#include <volymerna/Listener.h>

// internal
#include <volymerna/Source.h>
#include <volymerna/System.h>

// external

namespace volymerna {

Listener::Listener()
{
	System::Get()->listeners.push_back(this);
}

Listener::~Listener()
{
/*	for(uint i = 0; i < System::Get()->listeners.size(); ++i)
	{
		if(listeners[i] == this)
		{
			listeners.erase(listeners.begin()+i);
			break;
		}
	}*/
}

void Listener::Update()
{
	/// @todo If we've moved, recalculate volume and balance
	// if(position, rotation, or velocity is different) {
	//		do some calculations
	//		snd->Recalculate(volume, balance);
	// }
}

}
