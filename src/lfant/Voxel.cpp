/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-06-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

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

void Voxel::Chunk::BeginRender()
{
	blockBuffer.data.clear();
	for(uint x = 0; x < blocks.size(); ++x)
	{
		for(uint y = 0; y < blocks[x].size(); ++y)
		{
			for(uint z = 0; z < blocks[x][y].size(); ++z)
			{
				if(blocks[x][y][z].IsActive())
				{
					blockBuffer.push_back(vec3(x,y,z));
				}
			}
		}
	}
	
	if(blockBuffer.id)
	{
		glDeleteBuffers(1, &blockBuffer.id);
	}

	glGenBuffers(1, &blockBuffer.id);
	glBindBuffer(GL_ARRAY_BUFFER, blockBuffer);
	glBufferData(GL_ARRAY_BUFFER, blockBuffer.size(), &blockBuffer[0], GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Voxel::Chunk::Render()
{
//	glEnableVertexAttribute(0);
	glBindBuffer(GL_ARRAY_BUFFER, blockBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);

	glDrawArrays(GL_POINTS, 0, blockBuffer.size());
}

void Voxel::Chunk::EndRender()
{
	
}

// Voxel
IMPLEMENT_TYPE(Component, Voxel)

Voxel::Voxel() :
	shader(new Shader),
	texture(new Texture)
{
	chunks.emplace_back();
//	vec3 pos(0);
	uint32_t i1 = chunks.size()-1;
	uint32_t i2 = chunks[i1].size()-1;
	uint32_t i3 = chunks[i1][i2].size()-1;
	Chunk& chunk = chunks[i1][i2][i3];
	for(uint x = 0; x < chunk.blocks.size(); ++x)
	{
		for(uint y = 0; y < chunk.blocks[x].size(); ++y)
		{
			for(uint z = 0; z < chunk.blocks[x][y].size(); ++z)
			{
			//	vertexBuffer.emplace_back(x, y, z);
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
	shader->LoadFile("shaders/simple/Diffuse.vert", "shaders/simple/Diffuse.frag", "shaders/Voxel.geom");
	texture->LoadFile("textures/Default.png");

	shader->AddUniform("MVP");
	shader->AddUniform("textureSampler");
	shader->AddUniform("tiling");
	shader->AddUniform("chunkPos");

	for(uint x = 0; x < chunks.size(); ++x)
	{
		for(uint y = 0; y < chunks[x].size(); ++y)
		{
			for(uint z = 0; z < chunks[x][y].size(); ++z)
			{
				chunks[x][y][z].BeginRender();
			}
		}
	}
}

void Voxel::Render()
{
	shader->Bind();

	mat4 mvp = game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView() * owner->transform->GetMatrix();
	shader->SetUniform("MVP", mvp);
	shader->SetUniform("textureSampler", texture);
	shader->SetUniform("tiling", texture->tiling);

	for(uint x = 0; x < chunks.size(); ++x)
	{
		for(uint y = 0; y < chunks[x].size(); ++y)
		{
			for(uint z = 0; z < chunks[x][y].size(); ++z)
			{
				shader->SetUniform("chunkPos", vec3(x,y,z));
				chunks[x][y][z].Render();
			}
		}
	}

	shader->Unbind();
}

}
