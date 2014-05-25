
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

void Movement::Serialize(Properties* prop)
{
	Component::Serialize(prop);

	prop->Value("movementSpeed", &movementSpeed);
	prop->Value("jumpHeight", &jumpHeight);
	prop->Value("usePhysics", &usePhysics);
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
		owner->transform->Translate(velocity * movementSpeed * GetGame()->time->deltaTime);
	}
}

}
}
