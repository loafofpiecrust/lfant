/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-28 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/
#pragma once

// Internal
#include <lfant/Component.h>

// External

class btTypedConstraint;
class btRigidBody;
class btMotionState;
class btTransform;
class btVector3;
class btCollisionShape;

namespace lfant {

namespace physics {
	class MotionState;
}

class Collider;
class Mesh;


/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Physics
 *	 @{
 */

/**	This class manages physics functionality through the Physics class.
 *		Adds Rigidbody functionality to the Owner Entity, allowing it to
 *		be affected by gravity, forces, collision, constraints, etc. This
 *		makes it fully physics-interactive. For soft-bodies and cloth, look
 *		at the Softbody class.
 *	@todo
 *		Organize Rigidbody some... it's a bit messy.
 */
class Rigidbody : public Component
{
	friend class Physics;
	friend class physics::MotionState;
	DECLARE_COMP(Rigidbody)
public:

	// Nested classes
	enum class Mode : byte
	{
		Discrete,
		Continuous,
		ContinuousDynamic
	};

	enum class ColliderType : byte
	{
		Mesh,
		Box,
		Cylinder,
		Capsule
	};

	// ctor and dtor
	Rigidbody();
	virtual ~Rigidbody();

	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop);

	// Methods
	btTypedConstraint* GetConstraint(uint16_t idx);
	void RemoveConstraint(uint16_t idx);

	void ApplyForce(vec3 force, vec3 pos);
	void ApplyCentralForce(vec3 force);

	/**
	 *	Applies a constant acceleration.
	 */
	void Accelerate(vec3 force);

	/**
	 *	Returns the (current) mass of this Rigidbody.
	 */
	float GetMass();

	/**
	 *	Sets the mass of this Rigidbody.
	 */
	void SetMass(float mass);

	/**
	 *	Returns this Rigidbody's density.
	 *	\return mass / volume
	 */
	float GetDensity();

	/**
	 *	Sets the mass to (density * volume)
	 */
	void SetDensity(float density);

	/**
	 *	Returns the object's current velocity.
	 */
	vec3 GetVelocity();

	/**
	 *	Sets the velocity of this object directly.
	 */
	void SetVelocity(vec3 vel);

	/**
	 *	Returns the average of the three components of this object's velocity.
	 */
	float GetSpeed();

	float GetFriction() const;
	void SetFriction(float f);

	void SetMaxSpeed(float speed) { maxSpeed = speed; }
	float GetMaxSpeed() { return maxSpeed; }

	void SetTrigger(bool is);
	bool IsTrigger();

	// Variables
	Mode mode;

	bool useWorldGravity;
	bool usePointGravity;

	float drag = 1.0f;

	vec3 centerOfMass;

	float gravityMult = 1.0f;

protected:
	virtual void Init();
	virtual void Update();
	virtual void Deinit();

	// Slots
	void OnSetPos(vec3 pos);
	void OnSetRot(vec3 rot);
	void OnSetScale(vec3 scale);
	void OnSetMesh(Mesh* mesh);
	void OnSetCollider(Collider* collider);

	/// The current mass of this object, in kg.
	float mass = 1.0f;
	bool isTrigger = false;
	float maxSpeed = 0.0f;

	btRigidBody* body;
	btMotionState* motionState;
	Collider* collider = nullptr;
	vec3 inertia = vec3(0);

	bvec3 lockPosition = bvec3(false);
	bvec3 lockRotation = bvec3(false);
};

/** @} */
/** @} */
}
