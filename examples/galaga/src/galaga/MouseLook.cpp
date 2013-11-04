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

#include <galaga/MouseLook.h>

// Internal
#include <lfant/Transform.h>
#include <lfant/Time.h>
#include <lfant/Game.h>
#include <lfant/Renderer.h>
#include <lfant/Input.h>
#include <lfant/Console.h>

// External

namespace lfant {
namespace galaga {

IMPLEMENT_TYPE(lfant::Component, MouseLook)

MouseLook::MouseLook()
{
}

MouseLook::~MouseLook()
{

}

void MouseLook::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("lookSpeed", lookSpeed);
}

void MouseLook::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("lookSpeed", lookSpeed);
}

void MouseLook::Init()
{
	Component::Init();

//	ConnectEvent(SENDER(game->input, MouseMove), RECEIVER(this, OnSetMousePos));
	ConnectEvent(SENDER(owner, Look), RECEIVER(this, Look));
}

void MouseLook::Look(vec3 rot)
{
	if(owner->GetParent())
	{
		owner->GetParent()->transform->Rotate(vec3(0,0,rot.z) * lookSpeed * game->time->deltaTime);
		owner->transform->Rotate(vec3(rot.x,rot.y,0) * lookSpeed * game->time->deltaTime);

	//	Log("Parent rot: ", lexical_cast<string>(owner->GetParent()->transform->GetWorldRotation()));
	//	Log("Camera rot: ", lexical_cast<string>(owner->transform->GetWorldRotation()));
	}
	else
	{
		owner->transform->Rotate(rot * lookSpeed * game->time->deltaTime);
	}

}

void MouseLook::OnSetMousePos(ivec2 pos)
{
	ivec2 mousePos = pos-lastMouse;
	ivec2 screenCenter = game->renderer->GetResolution()/2;
	Look(vec3(mousePos.y, -mousePos.x, 0));
//	if(pos.x )
#if !ANDROID
	game->input->SetMousePos(screenCenter);
#endif
//	lastMouse = pos;
	lastMouse = screenCenter;
}

}
}