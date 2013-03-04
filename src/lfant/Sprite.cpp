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

#include <lfant/Engine.h>
#include <lfant/Renderer.h>
#include <lfant/Time.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Transform.h>
#include <lfant/Console.h>

namespace lfant
{

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::Init()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	bool shaderLoaded = false;
	if(material.shader.id == 0)
	{
		if(auto shader = game->renderer->GetShader(material.shader.name))
		{
			material.shader.id = shader.id;
			shaderLoaded = true;
		}
		else if(auto shader = game->renderer->GetShader("shaders/Diffuse"))
		{
			material.shader.id = shader.id;
			shaderLoaded = true;
		}
	}
	if(!shaderLoaded)
	{
		Log("Sprite::Init: Loading default shader");
		material.shader.LoadFile("shaders/Diffuse");
		game->renderer->AddShader(material.shader);
	}

	if(material.texture.id == 0)
	{
		material.texture.LoadFile(material.texture.name);
	}

	if(material.shader.id != 0)
	{
		// Get any uniforms here
		matrixId = glGetUniformLocation(material.shader.id, "MVP");
		material.texture.uniformId = glGetUniformLocation(material.shader.id, "textureSampler");
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

	vertexBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, vertexBuffer.data);
	uvBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, uvBuffer.data);
	indexBuffer.id = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.data);
}

void Sprite::Update()
{
	if(material.shader.id == 0 || material.texture.id == 0)
	{
		Init();
	}

	glBindVertexArray(vertexArray);

	glUseProgram(material.shader.id);

	mat4 mvp = game->scene->mainCamera->projection * game->scene->mainCamera->view * owner->GetComponent<Transform>()->GetMatrix();
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.texture.id);
	glUniform1i(material.texture.uniformId, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer.id);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/*
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer.id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	*/

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id);
	glDrawElements(GL_QUADS, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);

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
	glDeleteBuffers(1, &vertexBuffer.id);
	glDeleteBuffers(1, &uvBuffer.id);
	glDeleteBuffers(1, &indexBuffer.id);
	glDeleteTextures(1, &material.texture.id);
	glDeleteVertexArrays(1, &vertexArray);
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
