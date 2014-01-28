
// internal

// external

namespace volymerna {

class Listener
{
public:
	Listener();
	virtual ~Listener();

	virtual void Update();

	static deque<Listener*> listeners;
protected:

	vec3 position {0.0f};
	vec3 velocity {0.0f};

	float volume = 1.0f;
	float radius = 1.0f;

private:
};

}