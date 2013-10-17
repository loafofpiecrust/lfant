
#include <lfant/physics/MotionState.h>

// internal
#include <lfant/Transform.h>
#include <lfant/Console.h>

// external
#include <BulletDynamics/Dynamics/btRigidBody.h>

namespace lfant {
namespace physics {

MotionState::MotionState(Rigidbody* rb) :
	rigidbody(rb)
{
}

MotionState::~MotionState()
{
}

void MotionState::getWorldTransform(btTransform& worldTrans) const
{
	Log("MotionState::getWorldTransform: Touch.");
	worldTrans = btTransform::getIdentity();
}

void MotionState::setWorldTransform(const btTransform& trans)
{
	vec3 pos = vec3_cast<vec3>(trans.getOrigin());
	vec3 rot = eulerAngles(quat_cast<quat>(trans.getRotation()));

	vec3 oldPos = rigidbody->owner->transform->GetWorldPosition();
	vec3 oldRot = rigidbody->owner->transform->GetRotation();
	if(rigidbody->lockPosition.x)
	{
		pos.x = oldPos.x;
	}
	if(rigidbody->lockPosition.y)
	{
		pos.y = oldPos.y;
	}
	if(rigidbody->lockPosition.z)
	{
		pos.z = oldPos.z;
	}
	rigidbody->owner->transform->SetWorldPosition(pos);

	if(rigidbody->lockRotation.x)
	{
		rot.x = oldRot.x;
	}
	if(rigidbody->lockRotation.y)
	{
		rot.y = oldRot.y;
	}
	if(rigidbody->lockRotation.z)
	{
		rot.z = oldRot.z;
	}
	rigidbody->owner->transform->SetRotation(rot);

//	rigidbody->body->getWorldTransform().setOrigin(vec3_cast<btVector3>(pos));
//	rigidbody->body->getWorldTransform().setRotation(quat_cast<btQuaternion>(rigidbody->owner->transform->GetRotationQuat()));
}

}
}