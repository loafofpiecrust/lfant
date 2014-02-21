
// internal

// external
#include <lfant/ptr.h>
#include <deque>

namespace volymerna {

class Listener;
class Source;
class Sound;

class System
{
public:
	System();
	~System();

	static System* Get();

	void Init();

	float GetDeltaTime();

	float speedOfSound = 340.29f;

	std::deque<lfant::ptr<Listener>> listeners;
	std::deque<lfant::ptr<Source>> sources;

	std::deque<std::shared_ptr<Sound>> sounds;

private:
	float deltaTime = 0.01f;
	static System* instance;
};

}
