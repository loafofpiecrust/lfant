/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-06-22 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/PointLight.h>

// Internal
#include <lfant/Renderer.h>
#include <lfant/Shader.h>
#include <lfant/FrameBuffer.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/Transform.h>
#include <lfant/Console.h>
#include <lfant/FileSystem.h>
#include "lfant/Window.h"
#include <lfant/Input.h>
#include <lfant/Time.h>

// External
#include <GL/glew.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>

namespace lfant {

IMPLEMENT_TYPE(Component, PointLight)

PointLight::PointLight()
{
	color = vec3(0.5f);
	stencilShader = Shader::LoadFile("shaders/light/Point.vert", "shaders/Null.frag");
}

PointLight::~PointLight()
{
}

void PointLight::LoadFile(string path)
{
	GetGame()->Log("Loading mesh file '"+path+"'.");
	path = GetGame()->GetAssetPath(path).string();

	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	GetGame()->Log("Loaded mesh.. filling buffers");
	for(uint i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		posBuffer.reserve(posBuffer.size()+mesh->mNumVertices);
		uvBuffer.reserve(uvBuffer.size()+mesh->mNumVertices);
		indexBuffer.reserve(indexBuffer.size()+mesh->mNumFaces*3);

		for(uint k = 0; k < mesh->mNumVertices; ++k)
		{
			posBuffer.push_back(vec3_cast<vec3>(mesh->mVertices[k]));
			if(mesh->mTextureCoords[0] && mesh->mTextureCoords[0][k][0])
			{
				uvBuffer.push_back(vec2_cast<vec2>(mesh->mTextureCoords[0][k]));
			}
		}
		for(uint k = 0; k < mesh->mNumFaces; ++k)
		{
			for(uint j = 0; j < mesh->mFaces[k].mNumIndices; ++j)
			{
				indexBuffer.push_back(mesh->mFaces[k].mIndices[j]);
			}
		}
	}
	aiReleaseImport(scene);
}

void PointLight::Load(Properties* prop)
{
	Light::Load(prop);

	prop->Get("radius", radius);
	prop->Get("ambientIntensity", ambientIntensity);
	prop->Get("diffuseIntensity", diffuseIntensity);
	prop->Get("attenConst", attenConst);
	prop->Get("attenLinear", attenLinear);
	prop->Get("attenExp", attenExp);
	prop->Get("specularPower", specularPower);
	prop->Get("specularIntensity", specularIntensity);
}

void PointLight::Save(Properties* prop) const
{
	Light::Save(prop);

	prop->Set("radius", radius);
	prop->Set("ambientIntensity", ambientIntensity);
	prop->Set("diffuseIntensity", diffuseIntensity);
	prop->Set("attenConst", attenConst);
	prop->Set("attenLinear", attenLinear);
	prop->Set("attenExp", attenExp);
	prop->Set("specularPower", specularPower);
	prop->Set("specularIntensity", specularIntensity);
}

void PointLight::Init()
{
	Light::Init();

	LoadFile("meshes/sphere.obj");

	GetGame()->Log("PointLight::BeginRender()");

	shader = Shader::LoadFile("shaders/light/Point.vert", "shaders/light/Point.frag");

	// Shader uniforms
	shader->AddUniform("M");
	shader->AddUniform("VP");
	shader->AddUniform("lightPosition");
	shader->AddUniform("lightColor");
	shader->AddUniform("ambientIntensity");
	shader->AddUniform("diffuseIntensity");
	shader->AddUniform("attenuation");
	shader->AddUniform("specularPower");
	shader->AddUniform("specularIntensity");

//	shader->AddUniform("depthTex");
	shader->AddUniform("diffuseTex");
	shader->AddUniform("normalTex");
	shader->AddUniform("positionTex");
	shader->AddUniform("radius");

	shader->AddUniform("cameraPosition");
	shader->AddUniform("screenSize");

	Geometry::CreateBuffer(posBuffer, GL_ARRAY_BUFFER);
	Geometry::CreateBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER);
}

void PointLight::Update()
{
	float val = GetGame()->input->GetAxis("ShowFPS")->GetValue();
	if(val != 0.0f)
	{
		ambientIntensity += val * 3.0f * GetGame()->time->deltaTime;
		diffuseIntensity += val * 3.0f * GetGame()->time->deltaTime;

	//	std::cout << "light intensity: " << ambientIntensity << "\n";
	}

	val = GetGame()->input->GetAxis("MoveLight")->GetValue();
	if(val != 0.0f)
	{
		owner->transform->Translate({0.0f, 0.0f, val * GetGame()->time->deltaTime * 4.0f});
	}
}

void PointLight::Render()
{
	if(!shader)
	{
		return;
	}

	shader->Bind();

	shader->SetUniform("VP", GetGame()->scene->mainCamera->GetProjection() * GetGame()->scene->mainCamera->GetView());
	shader->SetUniform("M", glm::scale(owner->transform->GetMatrix(), vec3(radius)));
	shader->SetUniform("lightPosition", owner->transform->GetWorldPosition());
	shader->SetUniform("cameraPosition", GetGame()->scene->mainCamera->owner->transform->GetWorldPosition());
	shader->SetUniform("screenSize", (vec2)GetGame()->window->GetSize());
	shader->SetUniform("lightColor", color);
	shader->SetUniform("radius", radius);
	shader->SetUniform("ambientIntensity", ambientIntensity);
	shader->SetUniform("diffuseIntensity", diffuseIntensity);
	shader->SetUniform("attenuation", vec3(attenConst, attenLinear, attenExp));
	shader->SetUniform("specularPower", specularPower);
	shader->SetUniform("specularIntensity", specularIntensity);

//	shader->SetUniform("depthTex", game->renderer->frameBuffer->GetTexture("depthTex"));
	shader->SetUniform("diffuseTex", GetGame()->renderer->frameBuffer->GetTexture("diffuseTex"));
	shader->SetUniform("normalTex", GetGame()->renderer->frameBuffer->GetTexture("normalTex"));
	shader->SetUniform("positionTex", GetGame()->renderer->frameBuffer->GetTexture("positionTex"));

	InternalRender();

	shader->Unbind();
}

void PointLight::RenderStencil()
{
	if(!stencilShader)
	{
		return;
	}

	stencilShader->Bind();

	shader->SetUniform("VP", GetGame()->scene->mainCamera->GetProjection() * GetGame()->scene->mainCamera->GetView() * owner->transform->GetMatrix());

	InternalRender();

	stencilShader->Unbind();
}

void PointLight::InternalRender()
{
//	Update();
//	GetGame()->Log("Rendering point light");

	glActiveTexture(GL_TEXTURE0+2);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer.id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

//	glEnableVertexAttribArray(1);
//	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer.id);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id);
	glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);
//	glDrawArrays(GL_TRIANGLES, 0, posBuffer.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void PointLight::Deinit()
{
	Light::Deinit();
//	shader->Destroy();
}

}
