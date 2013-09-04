
#include <galaga/PongPlayer.h>

// external
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Input.h>
#include <lfant/Transform.h>
#include <lfant/Console.h>

// internal

using namespace lfant;

namespace pong {

IMPLEMENT_COMP(pong::Player)

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	rigidbody = owner->GetComponent<Rigidbody>();
	float t = game->time->GetTime();
	ConnectEvent(SENDER(owner, SetComponentRigidbody), &rigidbody);
	Log("pong::Player::Init: ConnectEvent took ", (game->time->GetTime() - t)*1000, " milliseconds");
}

void Player::Update()
{
	float val = 0.0f;

	val = game->input->GetAxis("Vertical");
	if(val != 0.0f)
	{
		if(rigidbody)
		{
			rigidbody->ApplyCentralForce(vec3(0, val, 0));
		}
		else
		{
			owner->transform->Translate(vec3(0, val, 0) * game->time->deltaTime);
		}
	}
	/*
	val = game->input->GetAxis("Horizontal");
	if(val != 0.0f)
	{
		if(rigidbody)
		{
			rigidbody->ApplyCentralForce(vec3(-val, 0, 0));
		}
		else
		{
			owner->transform->Translate(vec3(-val, 0, 0) * game->time->deltaTime);
		}
	}*/
}

void Player::OnDestroy()
{
}

}