
#include <lfant/stdafx.h>

// internal
#include <lfant/Component.h>

// external

namespace dftf {

class Player : public lfant::Component
{
	DECLARE_COMP(dftf::Player)
public:
	virtual void Init();
	virtual void Update();
	virtual void Deinit();
protected:

	float health = 5.0f;

private:
};

}