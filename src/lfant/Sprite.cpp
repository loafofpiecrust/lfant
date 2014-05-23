/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-28 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
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
#include <lfant/Geometry.h>

namespace lfant {

IMPLEMENT_TYPE(Component, Sprite)

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::Init()
{
	mesh->vertexBuffer.push_back(vec3(0, 0, 0));
	mesh->vertexBuffer.push_back(vec3(1, 0, 0));
	mesh->vertexBuffer.push_back(vec3(1, 1, 0));
	mesh->vertexBuffer.push_back(vec3(0, 1, 0));

	mesh->uvBuffer.push_back(vec2(0, 1));
	mesh->uvBuffer.push_back(vec2(1, 1));
	mesh->uvBuffer.push_back(vec2(1, 0));
	mesh->uvBuffer.push_back(vec2(0, 0));

	mesh->indexBuffer.push_back(0);
	mesh->indexBuffer.push_back(1);
	mesh->indexBuffer.push_back(2);
	mesh->indexBuffer.push_back(3);

	Mesh::Init();
}

void Sprite::Render()
{/*
	if(!material->shader->GetId() || !material->texture->GetId())
	{
		printf("Failing sprite rendering...\n");
	//	BeginRender();
		return;
	}

	printf("Rendering sprite...\n");

//	glUseProgram(material->shader->GetId());
	material->shader->Use();
	*/
//	mat4 mvp = game->scene->mainCamera->GetProjection() /*game->scene->mainCamera->GetView()*/ * owner->transform->GetMatrix();
/*	glUniformMatrix4fv(material->shader->GetUniform("MVP"), 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	material->texture->Bind();
	glUniform1i(material->shader->GetUniform("textureSampler"), 0);


	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer.id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_QUADS, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

//	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	material->texture->Unbind();
	*/
	Mesh::Render();
}

void Sprite::Update()
{
	Mesh::Update();
}

void Sprite::Load(Properties *props)
{
	GetGame()->Log("Loading a sprite from props");
//	Mesh::Load(props);
	string mat = "materials/Diffuse.mat";
	props->Get("material", mat);
//	material->texture->mode = GL_TEXTURE_RECTANGLE;
	material.LoadFile(mat);
	// Register Animation::Mode enum
/*
	props->SetEnum("Mode::Loop", Animation::Mode::Loop);
	props->SetEnum("Mode::Bounce", Animation::Mode::Bounce);
	props->SetEnum("Mode::Once", Animation::Mode::Once);
	props->SetEnum("Mode::Default", Animation::Mode::Default);
*/

//	deque<Properties*> panims = props->GetChildren("animation");
	for(Properties* pa : *props->GetChild("animations"))
	{
		string name = "";
		Animation anim;
		pa->Get("name", name);
		pa->Get("frameRate", anim.frameRate);

		if(!name.empty())
		{
			animations.insert({name, anim});
		}
	}
}

void Sprite::Deinit()
{
	Mesh::Deinit();
}

void Sprite::SetUV(uint32 idx, vec2 value)
{
	mesh->uvBuffer[idx] = value;
}

}
