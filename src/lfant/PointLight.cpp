/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2013-06-22 by Taylor Snead
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

// External
#include <GL/glew.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

namespace lfant {

IMPLEMENT_TYPE(Component, PointLight)

PointLight::PointLight()
{
}

PointLight::~PointLight()
{
}

void PointLight::Init()
{
	Light::Init();
	LoadFile("meshes/sphere.obj");
}

void PointLight::LoadFile(string path)
{	
	Log("Loading mesh file '"+path+"'.");
	path = game->fileSystem->GetGamePath(path).string();

	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	Log("Loaded mesh.. filling buffers");
	for(uint i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		posBuffer.data.reserve(posBuffer.size()+mesh->mNumVertices);
		uvBuffer.data.reserve(uvBuffer.size()+mesh->mNumVertices);
		indexBuffer.data.reserve(indexBuffer.size()+mesh->mNumFaces*3);

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

	BeginRender();
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

void PointLight::PostUpdate()
{
}

void PointLight::BeginRender()
{
	Light::BeginRender();

	shader->LoadFile("shaders/light/Point.vert", "shaders/light/Point.frag");

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

	Mesh::CreateBuffer(posBuffer, GL_ARRAY_BUFFER);
	Mesh::CreateBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER);
}

void PointLight::Render()
{
//	Log("Rendering point light");
	shader->Bind();

	shader->SetUniform("VP", game->scene->mainCamera->GetProjection() * game->scene->mainCamera->GetView());
	shader->SetUniform("M", owner->transform->GetMatrix());
	shader->SetUniform("lightPosition", owner->transform->GetWorldPosition());
	shader->SetUniform("cameraPosition", game->scene->mainCamera->owner->transform->GetWorldPosition());
	shader->SetUniform("screenSize", (vec2)game->renderer->GetResolution());
	shader->SetUniform("lightColor", color);
	shader->SetUniform("radius", radius);
//	shader->SetUniform("ambientIntensity", ambientIntensity);
//	shader->SetUniform("diffuseIntensity", diffuseIntensity);
//	shader->SetUniform("attenuation", vec3(attenConst, attenLinear, attenExp));
//	shader->SetUniform("specularPower", specularPower);
//	shader->SetUniform("specularIntensity", specularIntensity);

//	shader->SetUniform("depthTex", game->renderer->frameBuffer->GetTexture("depthTex"));
	shader->SetUniform("diffuseTex", game->renderer->frameBuffer->GetTexture("diffuseTex"));
	shader->SetUniform("normalTex", game->renderer->frameBuffer->GetTexture("normalTex"));
	shader->SetUniform("positionTex", game->renderer->frameBuffer->GetTexture("positionTex"));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, posBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);
//	glDrawArrays(GL_TRIANGLES, 0, posBuffer.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	shader->Unbind();
}

void PointLight::EndRender()
{
	Light::EndRender();
	shader->Destroy();
}

}