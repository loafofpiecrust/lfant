
#include <galaga/DftfPlayer.h>

// internal
#include <lfant/Game.h>
#include <lfant/Input.h>

// external

using namespace lfant;

namespace dftf {

IMPLEMENT_COMP(dftf::Player)

void Player::Init()
{
}

void Player::Update()
{
	float hor = game->input->GetAxis("Horizontal");
	float ver = game->input->GetAxis("Vertical");

	if(ver != 0.0f && hor != 0.0f)
	{
		hor = sqrt(hor*hor/2)*hor;
		ver = sqrt(ver*ver/2)*ver;
	}

	TriggerEvent("Move", vec3(hor,ver,0));
}

void Player::Deinit()
{
}

}