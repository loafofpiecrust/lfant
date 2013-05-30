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


namespace lfant
{

IMPLEMENT_COMP(Player)

void Player::Init()
{
	Component::Init();
	Log("Initing a player.");
	//	ConnectEvent(SENDER(game->input, Horizontal), this, &Player::Move);
	//	ConnectEvent(SENDER(game->input, Vertical), this, &Player::Move);
}

void Player::Move(string axis, float value)
{

}

void Player::Load(Properties *prop)
{
	Component::Load(prop);
	Log("Loading from player, '"+prop->type+" "+prop->id+"'.");
	prop->Get("movementSpeed", movementSpeed);
	prop->Get("lookSpeed", lookSpeed);
	prop->Get("mouseLook", mouseLook);

	Log("The setting of lookSpeed is ", prop->Get<float>("lookSpeed"));
}

void Player::Save(Properties *prop)
{
	Component::Save(prop);

	prop->Set("movementSpeed", movementSpeed);
	prop->Set("lookSpeed", lookSpeed);
	prop->Set("mouseLook", mouseLook);
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
		Log("Player position: ", owner->transform->GetPosition().x, ", ", owner->transform->GetPosition().y, ", ", owner->transform->GetPosition().z);
	}
	if (game->input->GetButtonDown("ShowRot"))
	{
		Log("Player rotation: ", owner->transform->GetRotation().x, ", ", owner->transform->GetRotation().y, ", ", owner->transform->GetRotation().z);
	}
	if(game->input->GetButtonDown("ShowFPS"))
	{
		Log("Game FPS: ", game->time->frameRate);
	}
	float hor = game->input->GetAxis("Horizontal");
	if (hor != 0.0f)
	{
		owner->transform->Translate(owner->transform->right * float(hor * game->time->deltaTime * movementSpeed));
	}
	float ver = game->input->GetAxis("Vertical");
	if (ver != 0.0f)
	{
		owner->transform->Translate(float(game->time->deltaTime) * owner->transform->direction * float(ver * movementSpeed));
	}
	float hrot = game->input->GetAxis("HRotation");
	if (hrot != 0.0f)
	{
		float input = hrot * lookSpeed;
		owner->transform->Rotate(vec3(0, 0, input * game->time->deltaTime));
	}
	float vrot = game->input->GetAxis("VRotation");
	if (vrot != 0.0f)
	{
		owner->transform->Rotate(vec3(vrot * lookSpeed * game->time->deltaTime, 0, 0));
	}
	if (game->input->GetButtonDown("Fire"))
	{
		Log("Player fired");
		/*
		Entity* ent = Entity::Spawn("TestMesh"+lexical_cast<string>(meshCount), nullptr, transform->GetPosition());
		Mesh* mesh = ent->AddComponent<Mesh>();
		mesh->LoadFile("suzanne.obj");
		mesh->material->texture.name = "player.png";
		mesh->material->shader->path = "Diffuse";
		*/
		dynamic_cast<Galaga*>(game)->AddMesh("TestMesh"+lexical_cast<string>(meshCount));
		++meshCount;
		Log("Added mesh ", meshCount);
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

}
