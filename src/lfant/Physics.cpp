/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-29 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Physics.h>

// External
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

// Internal
#include <lfant/physics/DebugRenderer.h>
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Scene.h>

#include <lfant/Console.h>
#include "Window.h"

#include <SFML/Graphics.hpp>


namespace lfant
{

Physics::Physics(Game* game) :
	Subsystem(game)
{
}

Physics::~Physics()
{
}

void Physics::Serialize(Properties* prop)
{
	Subsystem::Serialize(prop);

	vec3 grav {0.0f};

	if(prop->mode == Properties::Mode::Output)
		grav = GetGravity();

	prop->Value("gravity", &grav);

	if(prop->mode == Properties::Mode::Input)
		SetGravity(grav);

	auto add_pt_func = [](GravPoint& gp, Properties* prop)
	{
		prop->Value("name", &gp.name);
		prop->Value("force", &gp.force);
	};
	prop->ValueArray<GravPoint>("gravityPoint", gravityPoints, add_pt_func);

/*	Properties* gravPts = prop->Child("gravityPoints", "");
	if(gravPts)
	{
		for(auto& pgpt : gravPts->children)
		{
			GravPoint* gpt = new GravPoint;
			pgpt->Value("name", gpt->name);
			pgpt->Value("force", gpt->force);
			gravityPoints.push_back(*gpt);
		}
	}*/
}

/*******************************************************************************
*
*		Game Loop
*
*******************************************************************************/

void Physics::Init()
{
	collisionConfig = new btDefaultCollisionConfiguration();
	GetGame()->Log("Physics::Init: Collision config created.");
	dispatcher = new btCollisionDispatcher(collisionConfig);
	GetGame()->Log("Physics::Init: Collision dispatcher created.");
	broadphase = new btDbvtBroadphase();
	GetGame()->Log("Physics::Init: Broadphase created.");
	solver = new btSequentialImpulseConstraintSolver();
	GetGame()->Log("Physics::Init: Constraint solver created.");
	world = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, collisionConfig );

	world->setInternalTickCallback(&Physics::OnTick);

	debugRenderer = new physics::DebugRenderer;
	world->setDebugDrawer(debugRenderer);

//	gContactAddedCallback = &Physics::OnCollideEnter;
	gCollisionStartedCallback = &Physics::OnCollideEnter;
//	gContactProcessedCallback = &Physics::OnCollideStay;
//	gContactDestroyedCallback = &Physics::OnCollideExit;
	gCollisionEndedCallback = &Physics::OnCollideExit;

	GetGame()->Log("Physics::Init: Contact callbacks set.");

//	SetGravity(initGravity);

	LoadFile("settings/physics.prop");
}

void Physics::Update()
{
//	if(world)
	{
		world->stepSimulation(GetGame()->time->deltaTime, 1);
	//	world->stepSimulation(GetGame()->time->deltaTime, 0);
	//	world->stepSimulation(1.0f/100.0f, 1);
	}
}

void Physics::Render()
{
//	glDisable(GL_DEPTH_TEST);
//	GetGame()->window->GetHandle()->resetGLStates();
	world->debugDrawWorld();
	debugRenderer->Render(GetGame());
//	glEnable(GL_DEPTH_TEST);
}

void Physics::Deinit()
{
	Subsystem::Deinit();
	world.reset();
	solver.reset();
	broadphase.reset();
	dispatcher.reset();
	collisionConfig.reset();
}

/*******************************************************************************
*
*		Point Gravity
*
*******************************************************************************/

vec3 Physics::GetGravity() const
{
	return vec3_cast<vec3>(world->getGravity());
}

void Physics::SetGravity(vec3 grav)
{
	GetGame()->Log("Physics::SetGravity(", grav, ")");
	world->setGravity(vec3_cast<btVector3>(grav));
}

GravPoint* Physics::GetGravityPoint(string name)
{
	for(GravPoint& grav : gravityPoints)
	{
		if(grav.name == name)
		{
			return &grav;
		}
	}
	return nullptr;
}

void Physics::SetGravityPoint(string name, vec3 point, float force)
{
	if(GravPoint* grav = GetGravityPoint(name))
	{
		grav->point = new vec3(point);
		grav->force = force;
		return;
	}
	gravityPoints.push_back(GravPoint(name, new vec3(point), force));
}

void Physics::SetGravityPoint(string name, vec3 *point, float force)
{
	if(GravPoint* grav = GetGravityPoint(name))
	{
		grav->point = point;
		grav->force = force;
		return;
	}
	gravityPoints.push_back(GravPoint(name, point, force));
}

void Physics::SetGravityPoint(string name, float force)
{
	if(GravPoint* grav = GetGravityPoint(name))
	{
		grav->force = force;
		return;
	}
}

/*******************************************************************************
*
*		Static Collision Callbacks
*
*******************************************************************************/

void Physics::OnCollide(string func, btPersistentManifold* manifold)
{
	Rigidbody *body0 = reinterpret_cast<Rigidbody*>(manifold->getBody0()->getUserPointer());
	Rigidbody *body1 = reinterpret_cast<Rigidbody*>(manifold->getBody1()->getUserPointer());

/*	for (auto& rb : game->physics->bodies)
	{
		if (rb->body == colObj0->m_collisionObject)
		{
			GetGame()->Log("Body zero found.");
			body0 = rb;
		}
		else if (rb->body == colObj1->m_collisionObject)
		{
			GetGame()->Log("Body one found.");
			body1 = rb;
		}
	}*/

//	GetGame()->Log("Collision called, OnCollide", func, ", on entity ", body1->owner->GetName()+" and "+body0->owner->GetName());

	Collision col;
	col.contacts.push_back({vec3_cast<vec3>(manifold->getContactPoint(0).getPositionWorldOnA()), vec3_cast<vec3>(manifold->getContactPoint(0).getPositionWorldOnB())});

	col.first = body1;
	col.second = body0;
	body1->TriggerEvent("Collide"+func, &col);

	col.first = body0;
	col.second = body1;
	body0->TriggerEvent("Collide"+func, &col);

//	return true;
}

void Physics::OnCollideEnter(btPersistentManifold* manifold)
{
//	GetGame()->Log("Collision enter");
	OnCollide("Enter", manifold);
}

bool Physics::OnCollideStay(btManifoldPoint& cp, void* body0, void* body1)
{
	std::cout << ("Collision stay");
	//	return OnCollide( "Stay", cp, colObj0, partId0, index0, colObj1, partId1, index1 );
	return false;
}

void Physics::OnCollideExit(btPersistentManifold* manifold)
{
	std::cout << ("Collision exit\n");
	//	return OnCollide( "Exit", cp, colObj0, partId0, index0, colObj1, partId1, index1 );
//	return false;
}

void Physics::OnTick(btDynamicsWorld* world, float delta)
{
//	game->scene->FixedUpdate();
}

void Physics::AddRigidbody(Rigidbody* ent)
{
	GetGame()->Log("Adding rigidbody..");
	world->addRigidBody(ent->body);
	bodies.push_back(ent);
}

void Physics::RemoveRigidbody(Rigidbody* ent)
{
	world->removeRigidBody(ent->body);
	for(auto i = bodies.begin(); i != bodies.end(); ++i)
	{
		if(*i == ent)
		{
			bodies.erase(i);
			break;
		}
	}

}


}
