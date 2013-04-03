/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
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


// External
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

// Internal
#include <lfant/Collider.h>
#include <lfant/Physics.h>
#include <lfant/Transform.h>
#include <lfant/Game.h>
#include <lfant/Mesh.h>
#include <lfant/Console.h>
#include <lfant/Rigidbody.h>

namespace lfant
{

IMPLEMENT_COMP(Rigidbody)

Rigidbody::Rigidbody()
{
}

Rigidbody::~Rigidbody()
{
}

/*******************************************************************************
*		General functions
*		\area General
*******************************************************************************/

void Rigidbody::Init()
{
	Log("Rigidbody::Init: Touch.");
	motionState = new btDefaultMotionState;
	Log("Rigidbody::Init: Spawning underlying btRigidBody.");
	if(collider)
	{
		body = new btRigidBody(mass, motionState, collider->GetShape(), vec3_cast<btVector3>(inertia));
	}
	else
	{
		body = new btRigidBody(mass, motionState, new btEmptyShape, vec3_cast<btVector3>(inertia));
	}
	SetMass(mass);

	Log("Rigidbody::Init: Adding Rigidbody to physics system.");
	game->physics->AddRigidbody(this);

	ConnectEvent(SENDER(owner, SetPosition), RECEIVER(this, OnSetPos));
	ConnectEvent(SENDER(owner, SetRotation), RECEIVER(this, OnSetRot));
	ConnectEvent(SENDER(owner, SetCollider), RECEIVER(this, OnSetCollider));
}

void Rigidbody::Update()
{
}

void Rigidbody::OnDestroy()
{
//	game->physics->RemoveRigidbody(this);
}

/*******************************************************************************
*		Transform functions
*		\area Transform
*******************************************************************************/
void Rigidbody::OnSetPos( vec3 pos )
{
	body->getWorldTransform().setOrigin(vec3_cast<btVector3>(pos));
}

void Rigidbody::OnSetRot( vec3 rot )
{
//	body->getWorldTransform().setRotation( vec3_cast<btVector3>( rot ) );
}

/*******************************************************************************
*		Gravity functions
*		\area Gravity
*******************************************************************************/

float Rigidbody::GetMass()
{
	return mass;
}

void Rigidbody::SetMass(float mass)
{
	btVector3 temp;
	if(collider)
	{
		collider->GetShape()->calculateLocalInertia( mass, temp );
	}
	body->setMassProps(mass, temp);

	this->mass = mass;
	inertia = vec3_cast<vec3>(temp);

	TriggerEvent("SetMass", mass, inertia);
}

/*******************************************************************************
*		Constraints functions
*		\area Constraints
*******************************************************************************/

btTypedConstraint* Rigidbody::GetConstraint(uint16 idx)
{
	return body->getConstraintRef(idx);
}

void Rigidbody::RemoveConstraint(uint16 idx)
{
//	game->physics->RemoveConstraint( GetConstraint( idx ) );
}

float Rigidbody::GetSpeed()
{
	return length(GetVelocity());
}

vec3 Rigidbody::GetVelocity()
{
	return vec3_cast<vec3>(body->getLinearVelocity());
}

void Rigidbody::SetVelocity(vec3 vel)
{
	body->setLinearVelocity(vec3_cast<btVector3>(vel));
}

void Rigidbody::OnSetCollider(Collider *collider)
{
	delete body->getCollisionShape();
	body->setCollisionShape(collider->GetShape());
}

}
