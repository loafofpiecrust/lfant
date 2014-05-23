
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
//	GetGame()->Log("MotionState::getWorldTransform: Touch.");
	worldTrans.setOrigin(vec3_cast<btVector3>(rigidbody->GetOwner()->transform->GetWorldPosition()));
	worldTrans.setRotation(quat_cast<btQuaternion>(rigidbody->GetOwner()->transform->GetRotationQuat()));
}

void MotionState::setWorldTransform(const btTransform& trans)
{
	if(!rigidbody->IsEnabled()) return;

	vec3 pos = vec3_cast<vec3>(trans.getOrigin());
	quat rot = quat_cast<quat>(trans.getRotation());

	vec3 oldPos = rigidbody->owner->transform->GetWorldPosition();
	quat oldRot = rigidbody->owner->transform->GetRotationQuat();
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

//	if(pos != oldPos)
	{
	//	GetGame()->Log("MotionState::setWorldTransform(): new pos ", pos, " from ", oldPos);
		rigidbody->owner->transform->SetWorldPosition(pos);
	}

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
		rot.w = oldRot.w;
	}

//	if(rot != oldRot)
	rigidbody->owner->transform->SetRotationQuat(rot);

//	rigidbody->body->getWorldTransform().setOrigin(vec3_cast<btVector3>(pos));
//	rigidbody->body->getWorldTransform().setRotation(quat_cast<btQuaternion>(rigidbody->owner->transform->GetRotationQuat()));
}

}
}
