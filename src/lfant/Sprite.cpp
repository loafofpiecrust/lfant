/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
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
#include <lfant/Sprite.h>

// External
#include <GL/glew.h>

// Internal

#include <lfant/Game.h>
#include <lfant/Renderer.h>
#include <lfant/Time.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Transform.h>
#include <lfant/Console.h>

namespace lfant
{

IMPLEMENT_COMP(Sprite)

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::BeginRender()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	Log("Sprite vert arr bound");

	if(material->shader->GetId() == 0)
	{
		material->shader->LoadFile();
	}

	if(material->texture->GetId() == 0)
	{
		material->texture->LoadFile();
	}

	if(material->shader->GetId() != 0)
	{
		// Get any uniforms here
		material->shader->AddUniform("MVP");
		material->shader->AddUniform("textureSampler");
	}

	vertexBuffer.push_back(vec3(0, 0, 0));
	vertexBuffer.push_back(vec3(1, 0, 0));
	vertexBuffer.push_back(vec3(1, 1, 0));
	vertexBuffer.push_back(vec3(0, 1, 0));

	uvBuffer.push_back(vec2(0, 0));
	uvBuffer.push_back(vec2(1, 0));
	uvBuffer.push_back(vec2(1, 1));
	uvBuffer.push_back(vec2(0, 1));

	indexBuffer.push_back(0);
	indexBuffer.push_back(1);
	indexBuffer.push_back(2);
	indexBuffer.push_back(3);

	CreateBuffer(vertexBuffer, GL_ARRAY_BUFFER);
	CreateBuffer(uvBuffer, GL_ARRAY_BUFFER);
	CreateBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

	Renderable::BeginRender();
}

void Sprite::Render()
{
	if(!material->shader->GetId() || !material->texture->GetId())
	{
		printf("Failing sprite rendering...\n");
	//	BeginRender();
		return;
	}

	printf("Rendering sprite...\n");

	glBindVertexArray(vertexArray);

//	glUseProgram(material->shader->GetId());
	material->shader->Use();

	mat4 mvp = game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView() * owner->transform->GetMatrix();
	glUniformMatrix4fv(material->shader->GetUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material->texture->id);
	glUniform1i(material->shader->GetUniform("textureSampler"), 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/*
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer.id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_QUADS, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

//	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
}

void Sprite::Update()
{
	if(playingAnim)
	{
		if(currentAnim)
		{
			float x = 1.0f / currentAnim->columns;
			float y = 1.0f / currentAnim->rows;
			uvBuffer[0] = vec2(currentFrame.x + x, currentFrame.y);
			uvBuffer[1] = vec2(currentFrame.x, currentFrame.y);
			uvBuffer[2] = vec2(currentFrame.x, currentFrame.y + y);
			uvBuffer[3] = vec2(currentFrame.x, currentFrame.y + y);
			uvBuffer[4] = vec2(currentFrame.x + x, currentFrame.y + y);
			uvBuffer[5] = vec2(currentFrame.x + x, currentFrame.y);

			currentTime += game->time->deltaTime;
			if(currentTime >= currentAnim->frameRate)
			{
				currentTime = 0.0f;
				Animation::Mode mode = animMode;
				if(mode == Animation::Mode::Default)
				{
					if(currAnimMode == Animation::Mode::Default)
					{
						mode = currentAnim->mode;
					}
					else
					{
						mode = currAnimMode;
					}
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
						if(mode == Animation::Mode::Once)
						{
							playingAnim = false;
						}
						else if(mode == Animation::Mode::Bounce)
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
						if(mode == Animation::Mode::Once)
						{
							playingAnim = false;
						}
						else if(mode == Animation::Mode::Bounce)
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
	Renderable::Update();
}

void Sprite::Load(Properties *props)
{
	Log("Loading a sprite from props");
//	Mesh::Load(props);
	string mat = "materials/Diffuse.mat";
	props->Get("material", mat);
	material->LoadFile(mat);
	// Register Animation::Mode enum
	props->SetEnum("Mode::Loop", Animation::Mode::Loop);
	props->SetEnum("Mode::Bounce", Animation::Mode::Bounce);
	props->SetEnum("Mode::Once", Animation::Mode::Once);
	props->SetEnum("Mode::Default", Animation::Mode::Default);

	deque<Properties*> panims = props->GetChildren("animation");
	for(Properties*& pa : panims)
	{
		Animation anim;
		anim.name = pa->id;
		string mat = "materials/Diffuse.mat";
		anim.material.LoadFile(pa->Get("material", mat));
		pa->Get("frameRate", anim.frameRate);
		pa->GetEnum("playMode", anim.mode);
	}
}

void Sprite::EndRender()
{
	glDeleteBuffers(1, &vertexBuffer.id);
	glDeleteBuffers(1, &uvBuffer.id);
	glDeleteBuffers(1, &indexBuffer.id);
	glDeleteTextures(1, &material->texture->id);
	glDeleteVertexArrays(1, &vertexArray);

	Renderable::EndRender();
}

void Sprite::PlayAnim(string name, Animation::Mode mode, bool reverse)
{
	if(Animation* anim = &GetAnim(name))
	{
		currentAnim = anim;
		playingAnim = true;
		if(reverse)
		{
			playingReverseAnim = true;
		}
		currAnimMode = mode;
	}
}

Sprite::Animation& Sprite::GetAnim(string name)
{
	for(auto& anim : animations)
	{
		if(anim.name == name)
		{
			return anim;
		}
	}
}

void Sprite::PauseAnim()
{

}

}
