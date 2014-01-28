
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/Collider.h>

// external

class btSphereShape;

namespace lfant {

class SphereCollider : public Collider
{
	DECLARE_TYPE(Component, SphereCollider)
public:
	virtual void Init();

protected:
	btCollisionShape* GetShape();

	btSphereShape* shape = nullptr;

private:

};

}