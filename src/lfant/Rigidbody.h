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

// External

// Internal

#include <lfant/Component.h>

class btTypedConstraint;
class btRigidBody;
class btMotionState;
class btTransform;
class btVector3;
class btCollisionShape;

namespace lfant
{
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

public:
	// ctor and dtor
	Rigidbody();
	virtual ~Rigidbody();

	// Nested classes
	enum class PhysicsMode : byte
	{
		DISCRETE,
		CONTINUOUS,
		CONTINUOUS_DYNAMIC
	};

	enum class ColliderType : byte
	{
		MESH,
		BOX,
		CYLINDER,
		CAPSULE
	};

	// Methods
	btTypedConstraint* GetConstraint(uint16_t idx);
	void RemoveConstraint(uint16_t idx);

	void AddForce(float force);
	void AddForceAtPosition(float force, vec3 pos);

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

	// Variables
	PhysicsMode physicsMode;

	bool useWorldGravity;
	bool usePointGravity;

	float drag = 1.0f;

	vec3 centerOfMass;

	float gravityMult = 1.0f;

protected:
	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	// Slots
	void OnSetPos(vec3 pos);
	void OnSetRot(vec3 rot);
	void OnSetScale(vec3 scale);
	void OnSetMesh(Mesh* mesh);

	/// The current mass of this object, in kg.
	float mass = 0.0f;

	btRigidBody* body;
	btMotionState* motionState;
	btCollisionShape* collider;
	vec3 inertia;
};

/** @} */
/** @} */
}
