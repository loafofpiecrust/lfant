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
#include <lfant/Camera.h>

namespace lfant {
namespace galaga {

IMPLEMENT_COMP(Player)

void Player::Init()
{
	Component::Init();
	Log("Initing a player.");

	ConnectEvent(SENDER(game->input, Jump), RECEIVER(this, Jump));
	ConnectEvent(SENDER(game->input, Fire), RECEIVER(this, Fire));
	ConnectEvent(SENDER(game->input, Inventory), RECEIVER(this, ToggleInventory));
	ConnectEvent(SENDER(game->input, NextItem), RECEIVER(this, NextItem));
	ConnectEvent(SENDER(game->input, PreviousItem), RECEIVER(this, PreviousItem));
	ConnectEvent(SENDER(game->input, DropItem), RECEIVER(this, DropItem));
}

void Player::PreviousItem()
{
	TriggerEventWithChildren("EquipItem", 0u);
}

void Player::NextItem()
{
	TriggerEventWithChildren("EquipItem", 1u);
}

void Player::DropItem()
{
	TriggerEventWithChildren("RemoveItem", -1u);
}

void Player::Jump(float value)
{
	if(value == 1)
	{
		Log("Jumping");
		TriggerEvent("Jump");
	}
}

void Player::Fire(float value)
{
	if(value == 1.0f)
	{
		Log("Player fired");
		TriggerEventWithChildren("UseItem", (byte)0);
	}
	else
	{
		TriggerEventWithChildren("EndUseItem");
	}
}

void Player::Load(lfant::Properties *prop)
{
	Component::Load(prop);

	Log("Loading from player, '"+prop->type+" "+prop->id+"'.");
	prop->Get("lookSpeed", lookSpeed);

	Log("The setting of lookSpeed is ", lookSpeed);
}

void Player::Save(lfant::Properties *prop)
{
	Component::Save(prop);

	prop->Set("lookSpeed", lookSpeed);
}

void Player::Update()
{
	float value = 0.0f;
	if((value = game->input->GetAxis("Horizontal")) != 0.0f)
	{
		TriggerEvent("Move", owner->transform->GetRight() * -value);
	}
	if((value = game->input->GetAxis("Vertical")) != 0.0f)
	{
	//	TriggerEvent("Move", game->scene->mainCamera->owner->transform->GetDirection() * value);
		TriggerEvent("Move", owner->transform->GetDirection() * value);
	//	TriggerEvent("Move", vec3(0,0,value));
	}
	if((value = game->input->GetAxis("HRotation")) != 0.0f)
	{
		TriggerEventWithChildren("Look", vec3(0, value, 0));
	}
	if((value = game->input->GetAxis("VRotation")) != 0.0f)
	{
		TriggerEventWithChildren("Look", vec3(-value, 0, 0));
	}
	if((value = game->input->GetAxis("ZRotation")) != 0.0f)
	{
		TriggerEventWithChildren("Look", vec3(0, 0, value));
	}
	if (game->input->GetButtonDown("ShowLoc"))
	{
		Log("Player position: ", lexical_cast<string>(owner->transform->GetPosition()));
	}
	if (game->input->GetButtonDown("ShowRot"))
	{
	//	Log("Player rotation: ", lexical_cast<string>(owner->transform->GetRotation()));
	//	Log("Camera rotation: ", lexical_cast<string>(game->scene->mainCamera->owner->transform->GetRotation()));
	//	Log("Camera direction: ", lexical_cast<string>(game->scene->mainCamera->owner->transform->GetDirection()));
	//	Log("Camera right: ", lexical_cast<string>(game->scene->mainCamera->owner->transform->GetRight()));
	//	Log("Camera up: ", lexical_cast<string>(game->scene->mainCamera->owner->transform->GetUp()));
	//	Log("Camera matrix: ", lexical_cast<string>(game->scene->mainCamera->owner->transform->GetMatrix()));
	}
	if(game->input->GetButtonDown("ShowFPS"))
	{
		Log("deltaTime: ", game->time->deltaTime);
		Log("Game FPS: ", game->time->frameRate);
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
	if (game->input->GetButtonDown("Quit"))
	{
		Log("Calling exit function");
	//	game->console->Input("quit");
		game->Exit();
	}
	if(game->input->GetButtonDown("Reload"))
	{
		TriggerEventWithChildren("UseItem", (byte)1);
	}
}

}
}
