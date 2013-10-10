
#include <galaga/PongBall.h>

// internal

// external
#include <lfant/Input.h>
#include <lfant/Console.h>
#include <lfant/Transform.h>

using namespace lfant;

namespace pong {

IMPLEMENT_COMP(pong::Ball)

Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::Init()
{
//	ConnectEvent(SENDER(game->input, Jump_Down), RECEIVER(this, StartGame));
}

void Ball::Update()
{
	if(game->input->GetButtonDown("Jump"))
	{
		owner->TriggerEvent("ApplyCentralForce", vec3(100,100,0));
	}
	if(game->input->GetButtonDown("Fire"))
	{
		owner->transform->SetPosition(vec3(0,0,0));
	}
}

void Ball::FixedUpdate()
{
}

void Ball::StartGame()
{
	Log("Ball starting");
	owner->TriggerEvent("ApplyCentralForce", vec3(100,100,0));
}

}