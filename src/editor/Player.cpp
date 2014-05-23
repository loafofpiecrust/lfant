
#include <editor/Player.h>

// internal
#include <lfant/Game.h>
#include <lfant/Input.h>
#include <lfant/Scene.h>
#include <lfant/Transform.h>
#include <lfant/Camera.h>
#include <lfant/Console.h>
#include <lfant/Time.h>

// external

namespace lfant {
namespace editor {

IMPLEMENT_TYPE(Component, editor::Player)

void Player::Load(Properties* prop)
{
	Component::Load(prop);
}

void Player::Save(Properties* prop)
{
	Component::Save(prop);
}

void Player::Init()
{
	Component::Init();
}

void Player::Update()
{
	Component::Update();

//	Transform* cam = game->scene->mainCamera->GetOwner()->transform;

	float value = 0.0f;
	if((value = GetGame()->input->GetAxis("Horizontal")->GetValue()) != 0.0f)
	{
		TriggerEvent("Move", owner->transform->GetRight() * value);
	}
	if((value = GetGame()->input->GetAxis("Vertical")->GetValue()) != 0.0f)
	{
		TriggerEvent("Move", owner->transform->GetDirection() * value);
	}

	if((value = GetGame()->input->GetAxis("HRotation")->GetValue()) != 0.0f)
	{
		TriggerEvent("Look", vec3(0,0,1), value);
	}
	if((value = GetGame()->input->GetAxis("VRotation")->GetValue()) != 0.0f)
	{
		TriggerEvent("Look", vec3(1,0,0), -value);
	}
	if((value = GetGame()->input->GetAxis("ZRotation")->GetValue()) != 0.0f)
	{
		TriggerEvent("Look", vec3(0,1,0), -value);
	}

	if(GetGame()->input->GetButtonDown("ShowFPS"))
	{
		GetGame()->Log("deltaTime: ", GetGame()->time->deltaTime);
		GetGame()->Log("Game FPS: ", GetGame()->time->frameRate);
		GetGame()->Log("Object events: ", Object::GetEventCount());
	}

	if (GetGame()->input->GetButtonDown("ShowLoc"))
	{
		GetGame()->Log("Player position: ", owner->transform->GetPosition());
	}
	if (GetGame()->input->GetButtonDown("ShowRot"))
	{
		GetGame()->Log("Camera rotation: ", GetGame()->scene->mainCamera->owner->transform->GetWorldRotation());
		GetGame()->Log("Camera direction: ", GetGame()->scene->mainCamera->owner->transform->GetDirection());
		GetGame()->Log("Camera matrix: ", GetGame()->scene->mainCamera->owner->transform->GetMatrix());
	}
}

void Player::Deinit()
{
	Component::Deinit();
}

}
}
