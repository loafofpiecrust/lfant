
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
	worldTrans.setOrigin(vec3_cast<btVector3>(rigidbody->GetOwner()->transform->GetWorldPosition()));
	worldTrans.setRotation(quat_cast<btQuaternion>(rigidbody->GetOwner()->transform->GetRotationQuat()));
}

void MotionState::setWorldTransform(const btTransform& trans)
{
	if(!rigidbody->IsEnabled()) return;

	dvec3 pos = vec3_cast<dvec3>(trans.getOrigin());
	quat rot = quat_cast<quat>(trans.getRotation());

//	vec3 oldPos = rigidbody->owner->transform->GetWorldPosition();
//	quat oldRot = rigidbody->owner->transform->GetRotationQuat();

//	if(pos != oldPos)
	{
		rigidbody->owner->transform->SetWorldPosition(pos);
	}

//	if(rot != oldRot)
	rigidbody->owner->transform->SetRotationQuat(rot);

//	rigidbody->body->getWorldTransform().setOrigin(vec3_cast<btVector3>(pos));
//	rigidbody->body->getWorldTransform().setRotation(quat_cast<btQuaternion>(rigidbody->owner->transform->GetRotationQuat()));
}

}
}
