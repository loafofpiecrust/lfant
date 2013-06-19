/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2013-06-08 by Taylor Snead
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

#include <galaga/Movement.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Transform.h>
#include <lfant/Rigidbody.h>
#include <lfant/Console.h>

// External

namespace lfant {
namespace galaga {

IMPLEMENT_COMP(Movement)

Movement::Movement()
{
}

Movement::~Movement()
{
}

void Movement::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("movementSpeed", movementSpeed);
	prop->Set("jumpHeight", jumpHeight);
	prop->Set("usePhysics", usePhysics);
}

void Movement::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("movementSpeed", movementSpeed);
	prop->Get("jumpHeight", jumpHeight);
	prop->Get("usePhysics", usePhysics);
}

void Movement::Init()
{
	Component::Init();

	ConnectEvent(SENDER(owner, Move), RECEIVER(this, Move));
	ConnectEvent(SENDER(owner, Jump), RECEIVER(this, Jump));

	rigidbody = owner->GetComponent<Rigidbody>();
	ConnectEvent(SENDER(owner, SetComponentRigidbody), rigidbody);
}

void Movement::Update()
{
	Component::Update();
}

void Movement::Move(vec3 velocity)
{
	if(rigidbody && usePhysics)
	{
	//	rigidbody->Accelerate(velocity * movementSpeed);
		TriggerEvent("Accelerate", velocity * movementSpeed);
	}
	else
	{
		owner->transform->Translate(velocity * movementSpeed * game->time->deltaTime);
	}
}

void Movement::Jump()
{
//	Log("Jumpheight: ", jumpHeight / game->time->deltaTime);
	if(usePhysics)
	{
		TriggerEvent("Accelerate", vec3(0, jumpHeight / game->time->deltaTime, 0));
	}
	else
	{
		Move(owner->transform->GetUp());
	}
}

}
}