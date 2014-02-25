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

Rigidbody::Rigidbody()
{
	initBeforeLoad = true;
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::Save(Properties* prop) const
{
	Component::Save(prop);

	prop->Set("mass", mass);
	prop->Set("velocity", GetVelocity());
	prop->Set("maxSpeed", maxSpeed);
	prop->Set("trigger", IsTrigger());
	prop->Set("friction", body->getFriction());
	prop->Set("restitution", body->getRestitution());

	prop->Set("lockPosition", lockPosition);
	prop->Set("lockRotation", lockRotation);
}

void Rigidbody::Load(Properties* prop)
{
	Component::Load(prop);

	float friction = 1.0f, restitution = 1.0f, mass = 1.0f;
	vec3 vel(0);

	prop->Get("mass", mass);
	prop->Get("velocity", vel);
	prop->Get("maxSpeed", maxSpeed);
	prop->Get("trigger", isTrigger);
	prop->Get("friction", friction);
	prop->Get("restitution", restitution);

	prop->Get("lockPosition", lockPosition);
	prop->Get("lockRotation", lockRotation);

	Log("Rb loaded, restitution: ", restitution);

	if(body)
	{
		body->setFriction(friction);
		body->setRestitution(restitution);
		SetMass(mass);
		body->setLinearVelocity(vec3_cast<btVector3>(vel));
	}

	Log("Locked position: ", lexical_cast<string>(lockPosition));

//	SetVelocity(prop->Get<vec3>("velocity"));
}

/*******************************************************************************
*
*		General functions
*
*******************************************************************************/

void Rigidbody::Init()
{
	Component::Init();

	Log("Rigidbody::Init: Touch. Entity name: ", owner->GetName());
	motionState = new physics::MotionState(this);
	Log("Rigidbody::Init: Spawning underlying btRigidBody.");
	if((collider = owner->GetComponent<Collider>()))
	{
		Log("Rigidbody::Init: With collider.");
		body = new btRigidBody(mass, motionState, collider->GetShape(), vec3_cast<btVector3>(inertia));
	}
	else
	{
		Log("Rigidbody::Init: Without collider.");
		body = new btRigidBody(mass, motionState, new btEmptyShape, vec3_cast<btVector3>(inertia));
	}
	Log("Rigidbody::Init: Setting mass.");
	SetMass(mass);

	Log("Rigidbody::Init: Adding Rigidbody to physics system.");
	game->physics->AddRigidbody(this);

	// reactions
	ConnectEvent(SENDER(owner, SetPosition), RECEIVER(this, OnSetPos));
	ConnectEvent(SENDER(owner, SetRotation), RECEIVER(this, OnSetRot));
	ConnectEvent(SENDER(owner, AddComponent), RECEIVER(this, OnAddComponent));
	ConnectEvent(SENDER(owner, RemoveComponent), RECEIVER(this, OnRemoveComponent));

	// actions
	ConnectEvent(SENDER(owner, ApplyForce), RECEIVER(this, ApplyForce));
	ConnectEvent(SENDER(owner, ApplyCentralForce), RECEIVER(this, ApplyCentralForce));
	ConnectEvent(SENDER(owner, Accelerate), RECEIVER(this, Accelerate));
	ConnectEvent(SENDER(owner, SetTrigger), RECEIVER(this, SetTrigger));

	body->getWorldTransform().setOrigin(vec3_cast<btVector3>(owner->transform->GetWorldPosition()));
	body->forceActivationState(DISABLE_DEACTIVATION);
	body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
	Log("Rigidbody is trigger? ", IsTrigger());
	if(IsTrigger()) SetTrigger(true);
	
	body->setUserPointer(this);
}

void Rigidbody::Update()
{
	/*
	vec3 pos = vec3_cast<vec3>(body->getWorldTransform().getOrigin());
	if(pos != owner->transform->GetPosition())
	{
		owner->transform->SetPosition(pos);
	}
	*/
	/*
	vec3 rot = degrees(eulerAngles(quat_cast<quat>(body->getWorldTransform().getRotation())));
	Log(lexical_cast<string>(rot));
	if(rot != owner->transform->GetRotation())
	{
		owner->transform->SetRotation(rot);
	}

	/*
	quat rot = quat_cast<quat>(body->getWorldTransform().getRotation());
	if(rot != owner->transform->GetRotationQuat())
	{
		owner->transform->SetRotationQuat(rot);
	}
	*/
}

void Rigidbody::Deinit()
{
//	game->physics->RemoveRigidbody(this);
}

void Rigidbody::Enable(bool on)
{
	Component::Enable(on);
	if(on)
	{
		body->forceActivationState(DISABLE_DEACTIVATION);
		body->setGravity(vec3_cast<btVector3>(game->physics->GetGravity()));
	}
	else
	{
		Log("Disabling rigidbody on ", GetOwner());
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
//	Log("OnSetPos: Touch.");
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
//	Log("Setting rot");
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

	TriggerEvent("SetMass", mass, inertia);
}

/*******************************************************************************
*
*		Constraints functions
*
*******************************************************************************/

btTypedConstraint* Rigidbody::GetConstraint(uint16 idx)
{
	return body->getConstraintRef(idx);
}

void Rigidbody::RemoveConstraint(uint16 idx)
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
	TriggerEvent("OnSetVelocity", vel);
}

void Rigidbody::OnAddComponent(Component* comp)
{
	Collider* collider = dynamic_cast<Collider*>(comp);
	if(collider)
	{
		delete body->getCollisionShape();
		Log("Collider set for entity, setting for rigidbody, type: '", type::Name(collider), "'.");
		body->setCollisionShape(collider->GetShape());
		this->collider = collider;
		body->getWorldTransform().setOrigin(vec3_cast<btVector3>(owner->transform->GetWorldPosition()));
	}
}

void Rigidbody::OnRemoveComponent(Component* comp)
{
	Collider* collider = dynamic_cast<Collider*>(comp);
	if(collider)
	{
		delete body->getCollisionShape();
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
//	Log("Applying central force: ", lexical_cast<string>(force));
	body->applyCentralForce(vec3_cast<btVector3>(force));
	TriggerEvent("OnApplyForce", force, vec3(0));
	TriggerEvent("OnSetVelocity", vec3_cast<vec3>(body->getLinearVelocity()));
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
	TriggerEvent("OnApplyForce", force, pos);
	TriggerEvent("OnSetVelocity", vec3_cast<vec3>(body->getLinearVelocity()));
}

void Rigidbody::Accelerate(vec3 force)
{
	ApplyCentralForce(force*GetMass());
}

void Rigidbody::SetTrigger(bool is)
{
	isTrigger = is;
	if(is)
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
