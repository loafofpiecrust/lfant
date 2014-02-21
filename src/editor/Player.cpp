
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
	if((value = game->input->GetAxis("Horizontal")->GetValue()) != 0.0f)
	{
		TriggerEvent("Move", owner->transform->GetRight() * value);
	}
	if((value = game->input->GetAxis("Vertical")->GetValue()) != 0.0f)
	{
		TriggerEvent("Move", owner->transform->GetDirection() * value);
	}

	if((value = game->input->GetAxis("HRotation")->GetValue()) != 0.0f)
	{
		TriggerEvent("Look", vec3(0,0,1), value);
	}
	if((value = game->input->GetAxis("VRotation")->GetValue()) != 0.0f)
	{
		TriggerEvent("Look", vec3(1,0,0), -value);
	}
	if((value = game->input->GetAxis("ZRotation")->GetValue()) != 0.0f)
	{
		TriggerEvent("Look", vec3(0,1,0), -value);
	}

	if(game->input->GetButtonDown("ShowFPS"))
	{
		Log("deltaTime: ", game->time->deltaTime);
		Log("Game FPS: ", game->time->frameRate);
		Log("Object events: ", Object::GetEventCount());
	}

	if (game->input->GetButtonDown("ShowLoc"))
	{
		Log("Player position: ", owner->transform->GetPosition());
	}
	if (game->input->GetButtonDown("ShowRot"))
	{
		Log("Camera rotation: ", game->scene->mainCamera->owner->transform->GetWorldRotation());
		Log("Camera direction: ", game->scene->mainCamera->owner->transform->GetDirection());
		Log("Camera matrix: ", game->scene->mainCamera->owner->transform->GetMatrix());
	}
}

void Player::Deinit()
{
	Component::Deinit();
}

}
}
