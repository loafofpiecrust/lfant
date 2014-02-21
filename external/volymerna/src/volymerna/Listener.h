
// internal
#include <volymerna/Sound.h>

#include <lfant/ptr.h>

// external
#include <glm/geometric.hpp>
#include <deque>

namespace volymerna {

class Listener
{
public:
	Listener();
	virtual ~Listener();

	virtual void Update();

//	static std::deque<Listener*> listeners;
//protected:

	glm::vec3 position {0.0f};
	glm::vec3 velocity {0.0f};

	float volume = 1.0f;
	float radius = 1.0f;

private:
};

}
