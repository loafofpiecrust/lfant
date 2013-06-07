/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2013-01-11 by Taylor Snead
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

#include <galaga/Player.h>

// External

// Internal
#include <galaga/Galaga.h>
#include <lfant/Input.h>
#include <lfant/Console.h>
#include <lfant/Time.h>
#include <lfant/Sprite.h>
#include <lfant/Entity.h>
#include <lfant/Renderer.h>
#include <lfant/Settings.h>
#include <lfant/Scene.h>
#include <lfant/Transform.h>
#include <lfant/Rigidbody.h>
#include <lfant/Random.h>

namespace lfant {
namespace galaga {

IMPLEMENT_COMP(Player)

void Player::Init()
{
	Component::Init();
	Log("Initing a player.");
	ConnectEvent(SENDER(owner, SetComponentRigidbody), RECEIVER(this, OnSetRigidbody));

	if(Rigidbody* rb = owner->GetComponent<Rigidbody>())
	{
		haveRigidbody = true;
	}
	//	ConnectEvent(SENDER(game->input, Horizontal), this, &Player::Move);
	//	ConnectEvent(SENDER(game->input, Vertical), this, &Player::Move);
}

void Player::Move(string axis, float value)
{

}

void Player::Load(lfant::Properties *prop)
{
	Component::Load(prop);

	Log("Loading from player, '"+prop->type+" "+prop->id+"'.");
	prop->Get("movementSpeed", movementSpeed);
	prop->Get("lookSpeed", lookSpeed);
	prop->Get("mouseLook", mouseLook);
	prop->Get("jumpHeight", jumpHeight);
	prop->Get("bulletSpeed", bulletSpeed);

	Log("The setting of lookSpeed is ", prop->Get<float>("lookSpeed"));
}

void Player::Save(lfant::Properties *prop)
{
	Component::Save(prop);

	prop->Set("movementSpeed", movementSpeed);
	prop->Set("lookSpeed", lookSpeed);
	prop->Set("mouseLook", mouseLook);
	prop->Set("jumpHeight", jumpHeight);
	prop->Set("bulletSpeed", bulletSpeed);
}

void Player::Update()
{
	if(mouseLook)
	{
		ivec2 mousePos = game->input->GetMousePos()-lastMouse;
		ivec2 screenRes = game->renderer->GetResolution();
		owner->transform->Rotate(vec3(lookSpeed * -mousePos.y, 0, lookSpeed * -mousePos.x) * (float)game->time->deltaTime);
	//	game->input->SetMousePos(screenRes.x/2, screenRes.y/2);
		lastMouse = game->input->GetMousePos();
	}

	if (game->input->GetButtonDown("ShowLoc"))
	{
		Log("Player position: ", lexical_cast<string>(owner->transform->GetPosition()));
		Entity* ent = game->scene->GetEntity("StarShip");
		Log("Object pos: ", lexical_cast<string>(ent->transform->GetPosition()));
	}
	if (game->input->GetButtonDown("ShowRot"))
	{
		Log("Player rotation: ", lexical_cast<string>(owner->transform->GetRotation()));
	}
	if(game->input->GetButtonDown("ShowFPS"))
	{
		Log("Game FPS: ", game->time->frameRate);
	}
	float hor = game->input->GetAxis("Horizontal");
	if (hor != 0.0f)
	{
	//	Log("Pressing hor axis. Positive? ", hor > 0.0f);
		if(haveRigidbody)
		{
			TriggerEvent("Accelerate", owner->transform->GetRight() * vec3(1,0,1) /* game->time->deltaTime*/ * movementSpeed * hor);
		}
		else
		{
			owner->transform->Translate(owner->transform->GetRight() * game->time->deltaTime * movementSpeed * hor);
		}
	}
	float ver = game->input->GetAxis("Vertical");
	if (ver != 0.0f)
	{
		if(haveRigidbody)
		{
			TriggerEvent("Accelerate", owner->transform->GetDirection() * vec3(1,0,1) /* game->time->deltaTime*/ * movementSpeed * ver);
		}
		else
		{
			owner->transform->Translate(owner->transform->GetDirection() * game->time->deltaTime * movementSpeed * ver);
		}
	}
	float hrot = game->input->GetAxis("HRotation");
	if (hrot != 0.0f)
	{
		owner->transform->Rotate(vec3(0, -hrot * lookSpeed * game->time->deltaTime, 0));
	}
	float vrot = game->input->GetAxis("VRotation");
	if (vrot != 0.0f)
	{
		owner->transform->Rotate(vec3(vrot * lookSpeed * game->time->deltaTime, 0, 0));
	}
	if(game->input->GetButtonDown("Jump"))
	{
		if(haveRigidbody)
		{
			TriggerEvent("Accelerate", vec3(0, jumpHeight / game->time->deltaTime, 0));
		}
	}
	if (game->input->GetButtonDown("Fire"))
	{
		Log("Player fired");
		Entity* ent = game->scene->Spawn("bullet"+lexical_cast<string>(random::Range(0,200)));
		ent->transform->SetPosition(owner->transform->GetPosition());
		ent->transform->SetRotation(owner->transform->GetRotation());
		Mesh* m = ent->AddComponent<Mesh>();
		m->material->LoadFile("materials/Diffuse.mat");
		m->LoadFile("meshes/suzanne.obj");
		Rigidbody* rb = ent->AddComponent<Rigidbody>();
		rb->Accelerate(owner->transform->GetDirection() * bulletSpeed / game->time->deltaTime);
	}
	if (game->input->GetButtonDown("TesterSetVar"))
	{
		game->console->Input("set Tester 62.76f");
	}
	if (game->input->GetButtonDown("TesterGetVar"))
	{
		game->console->Input("get Tester");
	}
	if (game->input->GetButtonDown("TesterHelpMe"))
	{
		game->console->Input("help Tester");
	}
	if (game->input->GetButtonDown("Quit"))
	{
		Log("Calling exit function");
		game->console->Input("quit");
	}
}

void Player::OnSetRigidbody(Component* comp)
{
	if(comp)
	{
		haveRigidbody = true;
		dynamic_cast<Rigidbody*>(comp)->SetMaxSpeed(20.0f);
	}
	else
	{
		haveRigidbody = false;
	}
}

}
}
