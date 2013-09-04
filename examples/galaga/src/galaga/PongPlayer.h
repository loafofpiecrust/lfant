
#include <lfant/Component.h>
#include <lfant/Rigidbody.h>

namespace pong {

class Player : public lfant::Component
{
	DECLARE_COMP(pong::Player)
public:
	Player();
	~Player();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

private:
	lfant::Rigidbody* rigidbody = nullptr;
};

}