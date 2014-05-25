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

#include <lfant/Movement.h>

// Internal
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Transform.h>
#include <lfant/Rigidbody.h>
#include <lfant/Console.h>
#include <lfant/Input.h>

// External

namespace lfant {

IMPLEMENT_TYPE(Component, Movement)

Movement::Movement()
{
}

Movement::~Movement()
{
}

void Movement::Serialize(Properties* prop)
{
	Component::Serialize(prop);

	prop->Value("movementSpeed", &movementSpeed);
	prop->Value("usePhysics", &usePhysics);
}

void Movement::Init()
{
	Component::Init();

	ConnectEvent(SENDER(owner, Move), RECEIVER(this, Move));

	rigidbody = owner->GetComponent<Rigidbody>();
	ConnectEvent(SENDER(owner, SetComponentRigidbody), (Component**)&rigidbody);
}

void Movement::Update()
{
	Component::Update();
}

void Movement::FixedUpdate()
{
}

void Movement::Move(vec3 velocity)
{
//	GetGame()->Log("Movement::Move(", velocity, "): final(", velocity*movementSpeed);
	if(rigidbody && usePhysics)
	{
		rigidbody->ApplyCentralForce(velocity * movementSpeed * GetGame()->time->deltaTime);
		//TriggerEvent("Accelerate", velocity * movementSpeed);
	}
	else
	{
		owner->transform->Translate(velocity * movementSpeed * GetGame()->time->deltaTime);
	}
}

}
