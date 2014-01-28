
#pragma once
#include <lfant/stdafx.h>

// internal
#include <lfant/Rigidbody.h>

// external
#include <LinearMath/btMotionState.h>

namespace lfant {
namespace physics {

class MotionState : public btMotionState
{
public:
	MotionState(Rigidbody* rb);
	virtual ~MotionState();

	virtual void getWorldTransform(btTransform& worldTrans) const;
	virtual void setWorldTransform(const btTransform& trans);

protected:
	Rigidbody* rigidbody;
private:
};

}
}