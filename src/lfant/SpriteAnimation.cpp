#include <lfant/SpriteAnimation.h>

// internal
#include <lfant/Game.h>
#include <lfant/Time.h>
#include <lfant/Sprite.h>

// external

namespace lfant {

IMPLEMENT_SUBTYPE(Component, SpriteAnimation, Animation)

SpriteAnimation::SpriteAnimation()
{
}

SpriteAnimation::~SpriteAnimation()
{
}

void SpriteAnimation::Play()
{

}

void SpriteAnimation::Pause()
{

}

void SpriteAnimation::Stop()
{

}

void SpriteAnimation::Init()
{
	ConnectComponent(sprite);
}

void SpriteAnimation::Update()
{
	if(playing)
	{
		if(currentAnim)
		{
			float x = 1.0f / currentAnim->columns;
			float y = 1.0f / currentAnim->rows;
			sprite->SetUV(0, vec2(currentFrame.x + x, currentFrame.y));
			sprite->SetUV(1, vec2(currentFrame.x, currentFrame.y));
			sprite->SetUV(2, vec2(currentFrame.x, currentFrame.y + y));
			sprite->SetUV(3, vec2(currentFrame.x, currentFrame.y + y));
			sprite->SetUV(4, vec2(currentFrame.x + x, currentFrame.y + y));
			sprite->SetUV(5, vec2(currentFrame.x + x, currentFrame.y));

			currentTime += game->time->deltaTime;
			if(currentTime >= currentAnim->frameRate)
			{
				currentTime = 0.0f;
				Clip::Mode mode = currentAnim->mode;
				if(!playingReverseAnim)
				{
					if(currentFrame.x + x < 1.0f)
					{
						currentFrame.x += x;
					}
					else if(currentFrame.y + y < 1.0f)
					{
						currentFrame.y += y;
						currentFrame.x = 0.0f;
					}
					else
					{
						if(mode == Clip::Mode::Once)
						{
							playing = false;
						}
						else if(mode == Clip::Mode::Bounce)
						{
							playingReverseAnim = true;
						}

						currentFrame.x = 0.0f;
						currentFrame.y = 0.0f;
					}
				}
				else
				{
					if(currentFrame.x - x >= 0.0f)
					{
						currentFrame.x -= x;
					}
					else if(currentFrame.y - y >= 0.0f)
					{
						currentFrame.y -= y;
						currentFrame.x = 1.0f - x;
					}
					else
					{
						if(mode == Clip::Mode::Once)
						{
							playing = false;
						}
						else if(mode == Clip::Mode::Bounce)
						{
							playingReverseAnim = false;
						}

						currentFrame.x = 1.0f - x;
						currentFrame.y = 1.0f - y;
					}
				}
			}
		}
	}
	Animation::Update();
}

}
