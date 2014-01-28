
#include <volymerna/Listener.h>

// internal
#include <volymerna/Source.h>

// external

namespace volymerna {

Listener::Listener()
{
	listeners.push_back(this);
}

Listener::~Listener()
{
	for(uint i = 0; i < listeners.size(); ++i)
	{
		if(listeners[i] == this)
		{
			listeners.erase(listeners.begin()+i);
			break;
		}
	}
}

void Listener::Update()
{
	for(auto& src : Source::sources)
	{
		float dist = glm::distance(position, src->position);
		/// @todo Determine source radius by current volume
		if(dist <= radius+src->radius)
		{
			// transmit audio to source
			src->PlayCurrentFrame(dist);
		}
	}
}

}