/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-29 by Taylor Snead
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

#include "Rigidbody.hpp"

// External

// Internal
#include "Collider.hpp"
#include "Physics.hpp"
#include "Engine.hpp"

namespace sfs
{

Rigidbody::Rigidbody()
{
}

Rigidbody::~Rigidbody()
{
}

/*******************************************************************************
 *		General functions
 *		@area General
 *******************************************************************************/

void Rigidbody::Init()
{
	motionState = new btDefaultMotionState(initTransform);
	collider->shape->calculateLocalInertia(mass, inertia);
//	game->physics->SetGravityPoint( inertia );
	body = new btRigidBody(mass, motionState, collider->shape, inertia);

//connect( SENDER(&owner->transform, SetPos), RECEIVER(this, OnSetPos) );
//connect( SENDER(&owner->transform, SetRot), RECEIVER(this, OnSetRot) );
}

void Rigidbody::Update()
{
}

void Rigidbody::OnAddComponent(Component* comp)
{
	if(checkType<Collider*>(comp))
	{
		collider = dynamic_cast<Collider*>(comp);
	}
}

/*******************************************************************************
 *		Transform functions
 *		@area Transform
 *******************************************************************************/
/*
 void Rigidbody::OnSetPos( vec3 pos )
 {
 body->getWorldTransform().setOrigin(vec3_cast<btVector3>(pos));
 }

 void Rigidbody::OnSetRot( vec3 rot )
 {
 //	body->getWorldTransform().setRotation( vec3_cast<btVector3>( rot ) );
 }
 */
/*******************************************************************************
 *		Gravity functions
 *		@area Gravity
 *******************************************************************************/

float Rigidbody::GetMass()
{
	return this->_mass;
}

void Rigidbody::SetMass(float mass)
{
	this->mass = mass;
//	collider->shape->calculateLocalInertia( mass, inertia );
	body->setMassProps(mass, inertia);
}

vec3 Rigidbody::GetWorldGravity()
{
	return vec3_cast<vec3>(game->physics->world->getGravity());
}

void Rigidbody::SetWorldGravity(vec3 gravity)
{
	body->setGravity(vec3_cast<btVector3>(gravity));
}

/*******************************************************************************
 *		Constraints functions
 *		@area Constraints
 *******************************************************************************/

btTypedConstraint* Rigidbody::GetConstraint(uint16_t idx)
{
	return body->getConstraintRef(idx);
}

void Rigidbody::RemoveConstraint(uint16_t idx)
{
//	game->physics->RemoveConstraint( GetConstraint( idx ) );
}

}
