/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-19 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*		http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/

#include <lfant/Mesh.h>

// External
#include <GL/glew.h>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>

// Internal
#include <lfant/util/String.h>
#include <lfant/Transform.h>
#include <lfant/Console.h>
#include <lfant/Renderer.h>
#include <lfant/MeshLoader.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>
#include <lfant/FileSystem.h>
#include <lfant/FrameBuffer.h>
#include <lfant/Thread.h>

namespace lfant
{

IMPLEMENT_COMP(Mesh)

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Load(Properties *prop)
{
	Component::Load(prop);

	prop->Get("path", file);

	if(Properties* pmat = prop->GetChild("material"))
	{
		material->Load(pmat);
	}
	else
	{
		Log("Loading material from file");
		string mat = "";
		prop->Get("material", mat);
		if(mat != "") material->LoadFile(mat);
	}

	LoadFile(file);
}

void Mesh::Save(Properties *prop)
{
	Component::Save(prop);

	prop->Set("path", file);

	material->Save(prop->AddChild("material"));
}

void Mesh::SetShape(string preset)
{
	if(preset == "Cube")
	{
	}
}

void Mesh::Init()
{
	Renderable::Init();
}

void Mesh::Update()
{
	Renderable::Update();
}

void Mesh::OnDestroy()
{
	Renderable::OnDestroy();
}

void Mesh::BeginRender()
{
	if(loaded)
	{
		EndRender();
	}

	FrameBuffer* fbo = FrameBuffer::GetCurrent();
	if(fbo && !fboQuad)
	{
		Log("Unbinding fbo for mesh init.");
		fbo->Unbind();
	}

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	if(material->shader->GetId() == -1)
	{
		Log("Loading default shader.");
		material->shader->LoadFile("shaders/simple/Diffuse.vert", "shaders/simple/Diffuse.frag");
	}

//	if(material->texture->GetId() == -1)
	{
//		Log("Manually loading texture.");
//		material->texture->LoadFile();
	}

	if(material->shader->GetId() != -1)
	{
		Log("Adding uniforms..");
		material->shader->AddUniform("M");
		material->shader->AddUniform("V");
		material->shader->AddUniform("P");
		material->shader->AddUniform("textureSampler");
		material->shader->AddUniform("tiling");
	//	material->shader->AddUniform("modelMatrix");
	}

//	IndexVBO();

	CreateBuffer(vertexBuffer, GL_ARRAY_BUFFER);
	CreateBuffer(uvBuffer, GL_ARRAY_BUFFER);
	CreateBuffer(normalBuffer, GL_ARRAY_BUFFER);
	CreateBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER);
	/*
	vertexBuffer.Create(GL_ARRAY_BUFFER);
	uvBuffer.Create(GL_ARRAY_BUFFER);
	normalBuffer.Create(GL_ARRAY_BUFFER);
	indexBuffer.Create(GL_ELEMENT_ARRAY_BUFFER);
	*/

	loaded = true;
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	if(fbo && !fboQuad)
	{
		Log("Rebinding fbo after mesh init.");
		fbo->Bind();
	}
}

void Mesh::Render()
{
	if(!material->shader->GetId() /*|| !material->texture->GetId()*/)
	{
		return;
	}

//	Log("Rendering mesh '", owner->name, "'.");

	glBindVertexArray(vertexArray);

//	glUseProgram(material->shader->GetId());
//	Log("Binding shader for ", owner->name);
	material->shader->Bind();

//	mat4 mvp = mat4(1);
	if(owner && usingCamera)
	{
		material->shader->SetUniform("P", game->scene->mainCamera->GetProjection());
		material->shader->SetUniform("V", game->scene->mainCamera->GetView());
		material->shader->SetUniform("M", owner->transform->GetMatrix());
	}

//	if(material->shader->GetUniform("modelMatrix"))
	{
//		material->shader->SetUniform("modelMatrix", );
	}

//	Log("mesh mvp: ", lexical_cast<string>(mvp));

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, material->texture->GetId());
//	glUniform1i(material->shader->GetUniform("textureSampler"), 0);
	if(material->texture->GetId() != -1)
	{
	//	Log("Setting texture uniforms.");
		material->shader->SetUniform("textureSampler", material->texture);
		material->shader->SetUniform("tiling", material->texture->tiling);
	}

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	material->texture->Unbind();

	glBindVertexArray(0);

//	material->shader->Unbind();
	glUseProgram(0);
}

void Mesh::EndRender()
{
	vertexBuffer.Destroy();
	uvBuffer.Destroy();
	normalBuffer.Destroy();
	indexBuffer.Destroy();
	material->texture->Destroy();
	glDeleteVertexArrays(1, &vertexArray);
	loaded = false;
}

uint32 Mesh::CreateBuffer(void* data, uint32 size, int target, int mode)
{
	if(mode == 0)
	{
		mode = GL_STATIC_DRAW;
	}
	uint32 id = 0;
	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, size, data, mode);
	return id;
}

void Mesh::LoadFile(string path)
{
	if(!material->loaded)
	{
		material->LoadFile("materials/Diffuse.mat");
	}
	
	Log("Loading mesh file '"+path+"'.");
	path = game->fileSystem->GetGamePath(path).string();

	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);
	Log("Loaded mesh.. filling buffers");
	for(uint i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		vertexBuffer.data.reserve(vertexBuffer.size()+mesh->mNumVertices);
		uvBuffer.data.reserve(uvBuffer.size()+mesh->mNumVertices);
		normalBuffer.data.reserve(normalBuffer.size()+mesh->mNumVertices);
		indexBuffer.data.reserve(indexBuffer.size()+mesh->mNumFaces*3);

		for(uint k = 0; k < mesh->mNumVertices; ++k)
		{
			vertexBuffer.push_back(vec3_cast<vec3>(mesh->mVertices[k]));
			if(mesh->mTextureCoords[0] && mesh->mTextureCoords[0][k][0])
			{
				uvBuffer.push_back(vec2_cast<vec2>(mesh->mTextureCoords[0][k]));
			}
			normalBuffer.push_back(vec3_cast<vec3>(mesh->mNormals[k]));
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
//	scene->mMeshes[0]->

	BeginRender();
}

/// @fixme Something is horribly wrong.
void Mesh::IndexVBO()
{
	indexBuffer.data.clear();
//	vector<uint32> indices;
	bool found = false;

	Log("INDEXING VBO.");
	
	for(uint i = 0; i < vertexBuffer.size(); ++i)
	{
		for(uint k = 0; k < indexBuffer.size(); ++k)
		{
			if(vertexBuffer[indexBuffer[k]] == vertexBuffer[i])
			{
				indexBuffer.push_back(indexBuffer[k]);
				found = true;
			}
		}
		if(found)
		{
			found = false;
			continue;
		}
		else
		{
			indexBuffer.push_back(i);
		}
	}
	
	/*
	for(uint i = 0; i < vertexBuffer.size(); ++i)
	{
		indexBuffer.push_back(i);
	}
	*/
}

void BufferBase::Destroy()
{
	glDeleteBuffers(1, &id);
}

}
