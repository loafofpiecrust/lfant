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
#include <lfant/Engine.h>
#include <lfant/Mesh.h>

#include <lfant/Rigidbody.h>

namespace lfant
{

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
	motionState = new btDefaultMotionState();
	SetMass(mass);
	body = new btRigidBody(mass, motionState, collider, vec3_cast<btVector3>(inertia));

	game->physics->AddRigidbody(this);

	Connect(SENDER(owner, SetMesh), RECEIVER(this, OnSetMesh));
	Connect(SENDER(owner->GetComponent<Transform>(), SetPosition), RECEIVER(this, OnSetPos));
	Connect(SENDER(owner->GetComponent<Transform>(), SetRotation), RECEIVER(this, OnSetRot));
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
	this->mass = mass;
	Trigger("SetMass", mass, inertia);
//	collider->shape->calculateLocalInertia( mass, inertia );
	body->setMassProps(mass, vec3_cast<btVector3>(inertia));
}

/*******************************************************************************
 *		Constraints functions
 *		\area Constraints
 *******************************************************************************/

btTypedConstraint* Rigidbody::GetConstraint(uint16_t idx)
{
	return body->getConstraintRef(idx);
}

void Rigidbody::RemoveConstraint(uint16_t idx)
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
	btVector3 gg;
	body->setLinearVelocity(vec3_cast<btVector3>(vel));
}

void Rigidbody::OnSetMesh(Mesh *mesh)
{
}

}
