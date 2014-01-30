
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

IMPLEMENT_TYPE(Component, Player)

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

	float value = 0.0f;
	if((value = game->input->GetAxis("Horizontal")) != 0.0f)
	{
		TriggerEvent("Move", owner->transform->GetRight() * value);
	}
	if((value = game->input->GetAxis("Vertical")) != 0.0f)
	{
		TriggerEvent("Move", owner->transform->GetDirection() * value);
	}

	if((value = game->input->GetAxis("HRotation")) != 0.0f)
	{
		TriggerEvent("Look", vec3(0, 0, value));
	}
	if((value = game->input->GetAxis("VRotation")) != 0.0f)
	{
		TriggerEvent("Look", vec3(-value, 0, 0));
	}
	if((value = game->input->GetAxis("ZRotation")) != 0.0f)
	{
		TriggerEvent("Look", vec3(0, -value, 0));
	}

	if(game->input->GetButtonDown("ShowFPS"))
	{
		Log("deltaTime: ", game->time->deltaTime);
		Log("Game FPS: ", game->time->frameRate);
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
