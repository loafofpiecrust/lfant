
#include <lfant/SphereCollider.h>

// internal

// external
#include <btBulletCollisionCommon.h>

namespace lfant {

IMPLEMENT_TYPE(Component, SphereCollider)

void SphereCollider::Init()
{
	shape = new btSphereShape(size.x);
	Collider::Init();
}

btCollisionShape* SphereCollider::GetShape()
{
	return shape;
}

}