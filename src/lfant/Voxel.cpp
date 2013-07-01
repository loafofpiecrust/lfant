/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-06-17 by Taylor Snead
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

#include <lfant/Voxel.h>

// internal
#include <lfant/Game.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Transform.h>

// external
#include <GL/glew.h>

namespace lfant {

// Block
Voxel::Block::Block()
{
}

Voxel::Block::~Block()
{
}

bool Voxel::Block::IsActive()
{
	return active;
}

void Voxel::Block::SetActive(bool value)
{
	active = value;
}

// Chunk
Voxel::Chunk::Chunk()
{
	blocks.resize(Chunk::Size);
	for(auto& y : blocks)
	{
		y.resize(Chunk::Size);
		for(auto& z : y)
		{
			z.resize(Chunk::Size);
		}
	}
}

Voxel::Chunk::~Chunk()
{
//	blocks.clear();
}

void Voxel::Chunk::Render()
{

}


// Voxel
IMPLEMENT_COMP(Voxel)

Voxel::Voxel() :
	shader(new Shader),
	texture(new Texture)
{
	chunks.emplace_back();
//	vec3 pos(0);
	for(uint x = 0; x < chunks[0].blocks.size(); ++x)
	{
		for(uint y = 0; y < chunks[0].blocks[x].size(); ++y)
		{
			for(uint z = 0; z < chunks[0].blocks[x][y].size(); ++z)
			{
				vertexBuffer.emplace_back(x, y, z);
			}
		}
	}
}

Voxel::~Voxel()
{
}

void Voxel::Init()
{
}

void Voxel::Update()
{
}

void Voxel::Generate()
{
}

void Voxel::BeginRender()
{
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	shader->LoadFile("shaders/simple/Diffuse.vert", "shaders/simple/Diffuse.frag");
	texture->LoadFile("textures/Default.png");

	shader->AddUniform("MVP");
	shader->AddUniform("textureSampler");
	shader->AddUniform("tiling");

	glGenBuffers(1, &chunkId);
	glBindBuffer(GL_VERTEX_ARRAY, chunkId);
	glBufferData(GL_VERTEX_ARRAY, sizeof(vec3) * vertexBuffer.size(), &vertexBuffer[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Voxel::Render()
{
	glBindVertexArray(vertexArray);
	shader->Bind();

	mat4 mvp = game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView() * owner->transform->GetMatrix();
	shader->SetUniform("MVP", mvp);
	shader->SetUniform("textureSampler", texture);
	shader->SetUniform("tiling", texture->tiling);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, chunkId);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vertexBuffer.size());

	shader->Unbind();
	glBindVertexArray(0);
}

}