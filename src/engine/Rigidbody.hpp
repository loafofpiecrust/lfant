/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

// Internal
#include "Component.hpp"
#include "Collider.hpp"

namespace sfs
{
	class Collider;

	/** @addtogroup Engine
	 *	 @{
	 */
	/** @addtogroup Components
	 *	 @{
	 */

	/**	This class manages physics functionality through the Physics class.
	 *	@details
	 *		Adds Rigidbody functionality to the Owner Entity, allowing it to
	 *		be affected by gravity, forces, collision, constraints, etc. This
	 *		makes it fully physics-interactive. For soft-bodies and cloth, look
	 *		at the Softbody class.
	 *	@todo
	 *		Organize Rigidbody some... it's a bit messy.
	 */
	class Rigidbody : public Component
	{
	protected:
		friend class Physics;

	public:
		// ctor and dtor
		Rigidbody();
		virtual ~Rigidbody();

		// Nested classes
		enum PhysicsMode
		{
			Discrete,
			Continuous,
			ContinuousDynamic
		};

		// Methods
		void SetPointGravity(float gravity);
		void SetWorldGravity(vec3 gravity);
		vec3 GetWorldGravity();
		float GetPointGravity();

		btTypedConstraint* GetConstraint(uint16_t idx);
		void RemoveConstraint(uint16_t idx);

		void AddForce(float force);
		void AddForceAtPosition(float force, vec3 pos);

		// Variables
		PhysicsMode physicsMode;

		bool useWorldGravity;
		bool usePointGravity;

		float drag;

		vec3 _centerOfMass;

		float gravityMult;

		Collider* collider;

	protected:
		virtual void Init();
		virtual void Update();

		virtual void OnAddComponent(Component* comp);

	private:
		// Properties backstage

		/**
		 *	Returns the (current) mass of this Rigidbody.
		 */
		float GetMass();

		/**
		 *	Sets the mass of this Rigidbody.
		 */
		void SetMass(float mass);

		/// The current mass of this object.
		float _mass;

		/**
		 *	Returns this Rigidbody's density.
		 *	@return mass / volume
		 */
		float GetDensity();

		/**
		 *	Sets the mass to (density * volume)
		 */
		void SetDensity(float density);

		/**
		 *	Returns the object's current velocity.
		 */
		vec3& GetVelocity();

		/**
		 *	Sets the velocity of this object directly.
		 */
		void SetVelocity(vec3 vel);

		/**
		 *	Returns the average of the three components of this object's velocity.
		 */
		float GetSpeed();

		// Slots
		void OnSetPos(vec3 pos);
		void OnSetRot(vec3 rot);
		void OnSetScale(vec3 scale);

		btRigidBody* body;
		btMotionState* motionState;
		btTransform initTransform;
		btVector3 inertia;

	public:

		// Properties
		PROP_RW(Rigidbody, mass, GetMass, SetMass)
		PROP_RW(Rigidbody, density, GetDensity, SetDensity)
		PROP_RW(Rigidbody, velocity, GetVelocity, SetVelocity)

		PROP_RO(Rigidbody, speed, GetSpeed)
		//PROP_RW(Rigidbody, centerOfMass, GetCenterOfMass, SetCenterOfMass);
		//PROP_RW(Rigidbody, useWorldGravity, GetUseWorldGravity, SetUseWorldGravity)
		//PROP_RW(bool usePointGravity, GetUsePointGravity, SetUsePointGravity)
	};

	/** @} */
	/** @} */
}
