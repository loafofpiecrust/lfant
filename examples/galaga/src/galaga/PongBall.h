
#include <lfant/Component.h>

namespace pong {

class Ball : public lfant::Component
{
	DECLARE_COMP(pong::Ball)
public:
	Ball();
	~Ball();

	virtual void Init();
	virtual void Update();
	virtual void FixedUpdate();

	void StartGame();
};

}