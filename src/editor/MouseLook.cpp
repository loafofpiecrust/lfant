
#include <editor/MouseLook.h>

// internal
#include <lfant/Game.h>
#include <lfant/Input.h>
#include <lfant/Transform.h>
#include <lfant/Renderer.h>
#include <lfant/Time.h>

// external

namespace lfant {
namespace editor {

IMPLEMENT_TYPE(Component, editor::MouseLook)

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

void MouseLook::Look(vec3 axis, float value)
{
/*	if(owner->GetParent())
	{
		owner->transform->Rotate(vec3(0,0,rot.z) * lookSpeed * game->time->deltaTime);
		owner->transform->Rotate(vec3(rot.x,rot.y,0) * lookSpeed * game->time->deltaTime);

	//	Log("Parent rot: ", lexical_cast<string>(owner->GetParent()->transform->GetWorldRotation()));
	//	Log("Camera rot: ", lexical_cast<string>(owner->transform->GetWorldRotation()));
	}
	else*/
	{
		owner->transform->Rotate(axis, value * lookSpeed * game->time->deltaTime);
	}

}

void MouseLook::OnSetMousePos(ivec2 pos)
{
	ivec2 mousePos = pos-lastMouse;
	ivec2 screenCenter = game->renderer->GetResolution()/2;
	Look({1,0,0}, mousePos.y);
	Look({0,0,1}, -mousePos.x);
//	Look(vec3(mousePos.y, -mousePos.x, 0));
//	if(pos.x )
#if !ANDROID
	game->input->SetMousePos(screenCenter);
#endif
//	lastMouse = pos;
	lastMouse = screenCenter;
}

}
}
