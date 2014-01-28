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

#include <lfant/Physics.h>

// External
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

// Internal
//#include <lfant/physics/DebugRenderer.h>
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Scene.h>

#include <lfant/Console.h>

namespace lfant
{

Physics::Physics()
{
}

Physics::~Physics()
{
}

void Physics::Save(Properties* prop) const
{
	Subsystem::Save(prop);

	prop->Set("gravity", GetGravity());

	for(auto& gpt : gravityPoints)
	{
		Properties* pgpt = prop->AddChild("gravityPoint");
		pgpt->id = gpt.name;
		pgpt->Set("force", gpt.force);
	}
}

void Physics::Load(Properties* prop)
{
	Subsystem::Load(prop);

	vec3 grav = GetGravity();
	prop->Get("gravity", grav);
	SetGravity(grav);

	for(auto& pgpt : prop->GetChildren("gravityPoint"))
	{
		GravPoint* gpt = new GravPoint;
		gpt->name = pgpt->id;
		pgpt->Get("force", gpt->force); 
	}
}

/*******************************************************************************
*
*		Game Loop
*
*******************************************************************************/

void Physics::Init()
{
	collisionConfig = new btDefaultCollisionConfiguration();
	Log("Physics::Init: Collision config created.");
	dispatcher = new btCollisionDispatcher(collisionConfig);
	Log("Physics::Init: Collision dispatcher created.");
	broadphase = new btDbvtBroadphase();
	Log("Physics::Init: Broadphase created.");
	solver = new btSequentialImpulseConstraintSolver();
	Log("Physics::Init: Constraint solver created.");
	world = new btDiscreteDynamicsWorld( dispatcher, broadphase, solver, collisionConfig );

	world->setInternalTickCallback(&Physics::OnTick);
//	debugRenderer = new physics::DebugRenderer;

//	gContactAddedCallback = &Physics::OnCollideEnter;
//	gContactProcessedCallback = &Physics::OnCollideStay;
//	gContactDestroyedCallback = &Physics::OnCollideExit;

	Log("Physics::Init: Contact callbacks set.");

//	SetGravity(initGravity);

	Subsystem::Init();
}

void Physics::Update()
{
//	if(world)
	{
		world->stepSimulation(game->time->deltaTime, 1);

		/*
		uint numManifolds = dispatcher->getNumManifolds();
		btPersistentManifold* manifold = nullptr;
		for(uint i = 0; i < numManifolds; ++i)
		{
			manifold = dispatcher->getManifoldByIndexInternal(i);

			Collision* col = new Collision;
			Rigidbody* rb0 = nullptr;
			Rigidbody* rb1 = nullptr;
			for (auto& rb : bodies)
			{
				if (rb->body == manifold->getBody0())
				{
					rb0 = rb;
				}
				else if (rb->body == manifold->getBody1())
				{
					rb1 = rb;
				}
			}
			col->other = rb0;

		//	Log("Collision called, OnCollideEnter on entity ", rb1->owner->name);

			uint numContacts = manifold->getNumContacts();
			for(uint k = 0; k < numContacts; ++k)
			{
				btManifoldPoint& pt = manifold->getContactPoint(k);
				if(pt.getDistance() <= 0.0f)
				{
					col->contacts.push_back(ContactPoint(vec3_cast<vec3>(pt.getPositionWorldOnA()), vec3_cast<vec3>(pt.m_normalWorldOnB)));
				}
			}

			rb1->TriggerEvent("CollideEnter", col);
			rb1->owner->TriggerEvent("CollideEnter", rb1, col);
		}
		*/
	}
}

void Physics::Deinit()
{
	Subsystem::Deinit();
//	world.reset();
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

bool Physics::OnCollide(string func, btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0,
						const btCollisionObjectWrapper* colObj1, int partId1, int index1)
{
	Rigidbody *body0 = nullptr;
	Rigidbody *body1 = nullptr;

	for (auto& rb : game->physics->bodies)
	{
		if (rb->body == colObj0->m_collisionObject)
		{
			Log("Body zero found.");
			body0 = rb;
		}
		else if (rb->body == colObj1->m_collisionObject)
		{
			Log("Body one found.");
			body1 = rb;
		}
	}

	Log("Collision called, OnCollide", func, ", on entity ", body1->owner->name);

	Collision col;
	col.contacts.push_back(ContactPoint(vec3_cast<vec3>(cp.getPositionWorldOnA()), vec3_cast<vec3>(cp.m_normalWorldOnB)));

	col.first = body1;
	col.second = body0;
	body1->TriggerEvent("Collide"+func, &col);

	col.first = body0;
	col.second = body1;
	body0->TriggerEvent("Collide"+func, &col);

	return true;
}

bool Physics::OnCollideEnter(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0,
							 const btCollisionObjectWrapper* colObj1, int partId1, int index1)
{
	Log("Collision enter");
	return OnCollide("Enter", cp, colObj0, partId0, index0, colObj1, partId1, index1);
}

bool Physics::OnCollideStay(btManifoldPoint& cp, void* body0, void* body1)
{
	Log("Collision stay");
	//	return OnCollide( "Stay", cp, colObj0, partId0, index0, colObj1, partId1, index1 );
	return false;
}

bool Physics::OnCollideExit(void* userPersistentData)
{
	Log("Collision exit");
	//	return OnCollide( "Exit", cp, colObj0, partId0, index0, colObj1, partId1, index1 );
	return false;
}

void Physics::OnTick(btDynamicsWorld* world, float delta)
{
	game->scene->FixedUpdate();
}

void Physics::AddRigidbody(Rigidbody* ent)
{
	Log("Adding rigidbody..");
	world->addRigidBody(ent->body);
	bodies.push_back(ent);
}

}
