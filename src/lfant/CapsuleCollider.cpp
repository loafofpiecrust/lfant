#include "CapsuleCollider.h"

#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

namespace lfant {

IMPLEMENT_SUBTYPE(Component, CapsuleCollider, Collider)

CapsuleCollider::CapsuleCollider() :
	shape {new btCapsuleShape(1.0f, 2.0f)}
{
}

CapsuleCollider::~CapsuleCollider()
{
}

void CapsuleCollider::SetRadius(float radius)
{
	shape->setImplicitShapeDimensions({radius, shape->getHalfHeight(), radius});
}

void CapsuleCollider::SetHeight(float height)
{
	float radius = shape->getRadius();
	shape->setImplicitShapeDimensions({radius, height*0.5f, radius});
}

btCollisionShape* CapsuleCollider::GetShape()
{
	return shape;
}

} // namespace lfant
