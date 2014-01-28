
// internal

// external
#include <deque>

namespace volymerna {

class System
{
public:
	System();
	~System();

	void Init();

	static System* Get();

	float speedOfSound = 340.29f;
protected:


private:
	static System* instance;
};

}