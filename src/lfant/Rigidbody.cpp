/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-29 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

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
#include <lfant/physics/MotionState.h>

namespace lfant {

IMPLEMENT_TYPE(Component, Rigidbody)

Rigidbody::Rigidbody() :
	motionState {new physics::MotionState(this)}
{
	body = new btRigidBody(mass, nullptr, new btEmptyShape, vec3_cast<btVector3>(inertia));
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Serialize(Properties* prop)
{
	Component::Serialize(prop);

	prop->Value("mass", &mass);
	prop->Value("maxSpeed", &maxSpeed);
	prop->Value("lockPosition", &lockPosition);
	prop->Value("lockRotation", &lockRotation);

	if(prop->mode == Properties::Mode::Input)
	{
		float friction = 0.0f, restitution = 1.0f;
		vec3 vel(0);

		prop->Value("velocity", &vel);
		prop->Value("isTrigger", &isTrigger);
		prop->Value("friction", &friction);
		prop->Value("restitution", &restitution);


		GetGame()->Log("Rb loaded, restitution: ", restitution);

		if(body)
		{
			body->setFriction(friction);
			body->setRestitution(restitution);
			SetMass(mass);
			body->setLinearVelocity(vec3_cast<btVector3>(vel));
			SetTrigger(isTrigger);
		}

		GetGame()->Log("Locked position: ", lexical_cast<string>(lockPosition));
	}
	else
	{
		prop->Value("velocity", GetVelocity());
		prop->Value("isTrigger", IsTrigger());
		prop->Value("friction", body->getFriction());
		prop->Value("restitution", body->getRestitution());
	}
}

/*******************************************************************************
*
*		General functions
*
*******************************************************************************/

void Rigidbody::Init()
{
	Component::Init();

	body->setMotionState(motionState);

	OnAddComponent(owner->GetComponent<Collider>());
	SetMass(mass);
	GetGame()->physics->AddRigidbody(this);

	// reactions
//	ConnectComponent(collider);

	ConnectEvent(SENDER(owner, SetPosition), RECEIVER(this, OnSetPos));
	ConnectEvent(SENDER(owner, SetRotation), RECEIVER(this, OnSetRot));
//	ConnectEvent(SENDER(owner, AddComponent), RECEIVER(this, OnAddComponent));
	ConnectEvent(SENDER(owner, RemoveComponent), RECEIVER(this, OnRemoveComponent));

	// actions
	ConnectEvent(SENDER(owner, ApplyForce), RECEIVER(this, ApplyForce));
	ConnectEvent(SENDER(owner, ApplyCentralForce), RECEIVER(this, ApplyCentralForce));
	ConnectEvent(SENDER(owner, Accelerate), RECEIVER(this, Accelerate));
	ConnectEvent(SENDER(owner, SetTrigger), RECEIVER(this, SetTrigger));

	body->forceActivationState(DISABLE_DEACTIVATION);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	GetGame()->Log("Rigidbody is trigger? ", IsTrigger());
	if(IsTrigger()) SetTrigger(true);

	body->setUserPointer(this);
	GetGame()->Log("Setting rb pos: ", owner->transform->GetPosition());
	body->getWorldTransform().setOrigin(vec3_cast<btVector3>(owner->transform->GetWorldPosition()));
	GetGame()->Log("Rigidbody initial pos: ", vec3_cast<vec3>(body->getWorldTransform().getOrigin()));
}

void Rigidbody::Update()
{
}

void Rigidbody::Deinit()
{
	GetGame()->physics->RemoveRigidbody(this);
//	game->physics->world->removeRigidBody(body);
}

void Rigidbody::Enable(bool on)
{
	Component::Enable(on);
	if(on)
	{
		body->forceActivationState(DISABLE_DEACTIVATION);
		body->setGravity(vec3_cast<btVector3>(GetGame()->physics->GetGravity()));
	}
	else
	{
		GetGame()->Log("Disabling rigidbody on ", GetOwner());
		body->forceActivationState(0);
		body->setGravity({0.0f,0.0f,0.0f});
	}
}


/*******************************************************************************
*
*		Transform functions
*
*******************************************************************************/

void Rigidbody::OnSetPos( vec3 pos )
{
/*	if(pos == vec3_cast<vec3>(body->getWorldTransform().getOrigin()))
	{
		return;
	}*/
//	GetGame()->Log("OnSetPos: Touch.");
	body->getWorldTransform().setOrigin(vec3_cast<btVector3>(pos));
}

void Rigidbody::OnSetRot( quat rot )
{/*
	btQuaternion q = quat_cast<btQuaternion>(quat(radians(rot)));
	if(q == body->getWorldTransform().getRotation())
	{
		return;
	}
	body->getWorldTransform().setRotation(q);*/
//	GetGame()->Log("Setting rot");
//	rot = radians(rot);
	body->getWorldTransform().setRotation(quat_cast<btQuaternion>(rot));
}

/*******************************************************************************
*
*		Gravity functions
*
*******************************************************************************/

float Rigidbody::GetMass() const
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

	TriggerEvent("MassChanged", mass, inertia);
}

/*******************************************************************************
*
*		Constraints functions
*
*******************************************************************************/

btTypedConstraint* Rigidbody::GetConstraint(int idx)
{
	return body->getConstraintRef(idx);
}

void Rigidbody::RemoveConstraint(int idx)
{
//	game->physics->RemoveConstraint( GetConstraint( idx ) );
}

float Rigidbody::GetSpeed() const
{
	return length(GetVelocity());
}

vec3 Rigidbody::GetVelocity() const
{
	return vec3_cast<vec3>(body->getLinearVelocity());
}

void Rigidbody::SetVelocity(vec3 vel)
{
	body->setLinearVelocity(vec3_cast<btVector3>(vel));
//	TriggerEvent("VelocityChanged", vel);
}

void Rigidbody::OnAddComponent(Component* comp)
{
	if(dynamic_cast<Rigidbody*>(comp)) return;
	if(!comp) return;

	Collider* collider = dynamic_cast<Collider*>(comp);
	if(collider)
	{
		if(typeid(*body->getCollisionShape()) == typeid(btEmptyShape)) delete body->getCollisionShape();
		GetGame()->Log("Collider set for entity, setting for rigidbody, type: '", type::Name(collider), "'.");
		body->setCollisionShape(collider->GetShape());
		this->collider = collider;
		body->getWorldTransform().setOrigin(vec3_cast<btVector3>(owner->transform->GetWorldPosition()));
	}
}

void Rigidbody::OnRemoveComponent(Component* comp)
{
	if(typeid(*comp) == typeid(Collider))
	{
	//	delete body->getCollisionShape();
		body->setCollisionShape(new btEmptyShape);
		this->collider = nullptr;
	}
}

void Rigidbody::ApplyCentralForce(vec3 force)
{
/*	if(maxSpeed > 0.0f)
	{
		btVector3 vel = body->getLinearVelocity();
		if(abs(vel[0]) >= maxSpeed) force.x = 0.0f;
		if(abs(vel[1]) >= maxSpeed) force.y = 0.0f;
		if(abs(vel[2]) >= maxSpeed) force.z = 0.0f;
	}*/
//	GetGame()->Log("Applying central force: ", lexical_cast<string>(force));
	body->applyCentralForce(vec3_cast<btVector3>(force));
	TriggerEvent("ForceApplied", force, vec3(0));
//	TriggerEvent("OnSetVelocity", vec3_cast<vec3>(body->getLinearVelocity()));
}

void Rigidbody::ApplyForce(vec3 force, vec3 pos)
{
/*	if(maxSpeed > 0.0f)
	{
		btVector3 vel = body->getLinearVelocity();
		if(abs(vel[0]) >= maxSpeed) force.x = 0.0f;
		if(abs(vel[1]) >= maxSpeed) force.y = 0.0f;
		if(abs(vel[2]) >= maxSpeed) force.z = 0.0f;
	}*/
	body->applyForce(vec3_cast<btVector3>(force), vec3_cast<btVector3>(pos));
	TriggerEvent("ForceApplied", force, pos);
//	TriggerEvent("OnSetVelocity", vec3_cast<vec3>(body->getLinearVelocity()));
}

void Rigidbody::Accelerate(vec3 force)
{
	ApplyCentralForce(force*GetMass());
}

void Rigidbody::SetTrigger(bool is)
{
	isTrigger = is;
	if(isTrigger)
	{
		body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	else
	{
		body->setCollisionFlags(body->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

bool Rigidbody::IsTrigger() const
{
	return isTrigger;
}

}
