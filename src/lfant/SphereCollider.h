
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/Collider.h>

// external

class btSphereShape;

namespace lfant {

class SphereCollider : public Collider
{
public:
	virtual void Init();

protected:
	btCollisionShape* GetShape();

	btSphereShape* shape = nullptr;

private:

};

}
