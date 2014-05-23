#include "Rotatable.h"

// internal
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Scene.h>
#include <lfant/Console.h>
#include <lfant/Input.h>
#include <lfant/Transform.h>

// external

namespace lfant {
namespace editor {

IMPLEMENT_TYPE(Component, editor::Rotatable)

Rotatable::Rotatable()
{
}

void Rotatable::Update()
{
	Component::Update();

	float value = 0.0f;

	vec3 worldX = vec3(1,0,0) + (vec3(1,0,0) - GetOwner()->transform->GetRight());
	vec3 worldY = vec3(0,1,0) + (vec3(0,1,0) - GetOwner()->transform->GetUp());
	vec3 worldZ = vec3(0,0,1) + (vec3(0,0,1) - GetOwner()->transform->GetDirection());

	if((value = GetGame()->input->GetAxis("HRotation2")->GetValue()) != 0.0f)
	{
	//	GetOwner()->transform->Rotate({0,0,1}, value * game->time->deltaTime * 5.0f);
		GetOwner()->transform->Rotate(worldZ, value * GetGame()->time->deltaTime * 5.0f);
	}
	if((value = GetGame()->input->GetAxis("VRotation2")->GetValue()) != 0.0f)
	{
	//	GetOwner()->transform->Rotate({1,0,0}, -value * game->time->deltaTime * 5.0f);
		GetOwner()->transform->Rotate(worldX, -value * GetGame()->time->deltaTime * 5.0f);
	}
	if((value = GetGame()->input->GetAxis("ZRotation2")->GetValue()) != 0.0f)
	{
	//	GetOwner()->transform->Rotate({0,1,0}, -value * game->time->deltaTime * 5.0f);
		GetOwner()->transform->Rotate(worldY, -value * GetGame()->time->deltaTime * 5.0f);
	}

	if(GetGame()->input->GetButtonDown("ShowRot"))
	{
		GetGame()->Log("Object rotation: ", GetOwner()->transform->GetRotation());
	}
}

} // namespace editor
} // namespace lfant
