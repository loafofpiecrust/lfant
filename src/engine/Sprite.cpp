/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-07-28 by Taylor Snead
 *
 *	Licensed under the Apache License, Version 2.0 (the "License");
 *	you may not use this file except in compliance with the License.
 *	You may obtain a copy of the License at
 *
 *	http://www.apache.org/licenses/LICENSE-2.0
 *
 *	Unless required by applicable law or agreed to in writing, software
 *	distributed under the License is distributed on an "AS IS" BASIS,
 *	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *	See the License for the specific language governing permissions and
 *	limitations under the License.
 *
 ******************************************************************************/
#include "Sprite.hpp"

// External

// Internal
#include "Engine.hpp"
#include "Renderer.hpp"
#include "Time.hpp"
#include "Console.hpp"

namespace sfs
{

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::Init()
{
	vertices.push_back(Vertex(vec3(1,1,0), vec2(1,1)));
	vertices.push_back(Vertex(vec3(1,-1,0), vec2(1,0)));
	vertices.push_back(Vertex(vec3(-1,-1,0), vec2(0,0)));
	vertices.push_back(Vertex(vec3(-1,1,0), vec2(0,1)));
	Log("Sprite: Initialized");
}

void Sprite::Update()
{
	game->renderer->RenderSprite(this);
	if(playingAnim)
	{
		if(currentAnim)
		{
			float x = 1.0f / currentAnim->columns;
			float y = 1.0f / currentAnim->rows;
			vertices[0].tex = vec2(currentFrame.x + x, currentFrame.y);
			vertices[1].tex = vec2(currentFrame.x, currentFrame.y);
			vertices[2].tex = vec2(currentFrame.x, currentFrame.y + y);
			vertices[3].tex = vec2(currentFrame.x, currentFrame.y + y);
			vertices[4].tex = vec2(currentFrame.x + x, currentFrame.y + y);
			vertices[5].tex = vec2(currentFrame.x + x, currentFrame.y);

			currentTime += game->time->deltaTime;
			if(currentTime >= currentAnim->frameRate)
			{
				currentTime = 0.0f;
				AnimPlayMode mode = animMode;
				if(mode == Default)
				{
					if(currAnimMode == Default)
					{
						mode = currentAnim->playMode;
					}
					else
					{
						mode = currAnimMode;
					}
				}
				if(mode == Default)
				{
					mode = Loop;
				}
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
						if(mode == Once)
						{
							playingAnim = false;
						}
						else if(mode == Bounce)
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
						if(mode == Once)
						{
							playingAnim = false;
						}
						else if(mode == Bounce)
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
}

void Sprite::OnDestroy()
{
	game->renderer->RemoveSprite(this);
}

void Sprite::PlayAnim(string name, AnimPlayMode mode, bool reverse)
{
	currentAnim = &animations[name];
	playingAnim = true;
	if(reverse)
	{
		playingReverseAnim = true;
	}
	currAnimMode = mode;
}

void Sprite::PauseAnim()
{

}

}
