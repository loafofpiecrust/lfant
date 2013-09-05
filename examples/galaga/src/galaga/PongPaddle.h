
#include <lfant/Component.h>

namespace pong {

class Paddle : public lfant::Component
{
public:
	Paddle();
	~Paddle();

	virtual void Init();
	virtual void Update();
};

}