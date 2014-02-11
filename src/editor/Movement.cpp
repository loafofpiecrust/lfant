
#include <editor/Movement.h>

// internal
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Transform.h>
#include <lfant/Rigidbody.h>
#include <lfant/Console.h>
#include <lfant/Input.h>

// external

namespace lfant {
namespace editor {

IMPLEMENT_TYPE(Component, editor::Movement)

void Movement::Load(Properties* prop)
{
	Component::Load(prop);

	prop->Get("movementSpeed", movementSpeed);
	prop->Get("jumpHeight", jumpHeight);
	prop->Get("usePhysics", usePhysics);
}

void Movement::Save(Properties* prop)
{
	Component::Save(prop);

	prop->Set("movementSpeed", movementSpeed);
	prop->Set("jumpHeight", jumpHeight);
	prop->Set("usePhysics", usePhysics);
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

void Movement::Deinit()
{
	Component::Deinit();
}

void Movement::Move(vec3 velocity)
{
	if(rigidbody && usePhysics)
	{
		rigidbody->Accelerate(velocity * movementSpeed);
	//	TriggerEvent("Accelerate", velocity * movementSpeed);
	}
	else
	{
		owner->transform->Translate(velocity * movementSpeed * game->time->deltaTime);
	}
}

}
}
