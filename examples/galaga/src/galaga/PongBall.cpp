
#include <galaga/PongBall.h>

// internal

// external
#include <lfant/Input.h>
#include <lfant/Console.h>

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
	ConnectEvent(SENDER(game->input, Jump_Down), RECEIVER(this, StartGame));
}

void Ball::Update()
{
}

void Ball::StartGame()
{
	Log("Ball starting");
	owner->TriggerEvent("ApplyCentralForce", vec3(100,100,0));
}

}