/*
*	Copyright (C) 2013-2014, by loafofpiecrust
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

#include <lfant/Geometry.h>

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

std::deque<std::weak_ptr<Geometry>> Geometry::instanceCache;
uint32 Geometry::currentVertArray;

Geometry::Geometry()
{
//	initBeforeLoad = false;
}

Geometry::~Geometry()
{
	Deinit();
}

void Geometry::Init()
{
	if(vertexBuffer.size() == 0)
	{
	//	GetGame()->Log("Mesh init rejected (no data)");
		return;
	}

	if(loaded)
	{
	//	GetGame()->Log("Mesh init rejected (previously loaded)");
		return;
	}

	FrameBuffer* fbo = FrameBuffer::GetCurrent();
	if(fbo && !fboQuad)
	{
		fbo->Unbind();
	}

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	CreateBuffer(vertexBuffer, GL_ARRAY_BUFFER);
	CreateBuffer(uvBuffer, GL_ARRAY_BUFFER);
	CreateBuffer(normalBuffer, GL_ARRAY_BUFFER);
	CreateBuffer(indexBuffer, GL_ELEMENT_ARRAY_BUFFER);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer.id);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer.id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/// @todo Do this if this class will stay for static meshes
	/*
	vertexBuffer.clear();
	uvBuffer.clear();
	normalBuffer.clear();
	indexBuffer.clear();
	*/

	if(fbo && !fboQuad)
	{
		fbo->Bind();
	}

//	GetGame()->Log(glGetError());

	loaded = true;

//	Renderable::Init();
}

void Geometry::Deinit()
{
	vertexBuffer.Destroy();
	uvBuffer.Destroy();
	normalBuffer.Destroy();
	indexBuffer.Destroy();
	loaded = false;
//	Renderable::Deinit();
}

void Geometry::Update()
{

}

void Geometry::Render()
{
	if(!loaded)
	{
		return;
	}

	glBindVertexArray(vertexArray);
	glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);

/*	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);*/

	return;

/*	if(currentVertArray != vertexArray)
	{
		currentVertArray = vertexArray;
	}
	else
	{
		glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);
	}*/
}

uint32 Geometry::CreateBuffer(void* data, uint32 size, int target, int mode)
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

std::shared_ptr<Geometry> Geometry::LoadFile(string path)
{
	if(path.empty()) return nullptr;

	for(auto& inst : Geometry::instanceCache)
	{
		auto mesh = inst.lock();
		if(mesh->file == path)
		{
			return mesh;
		}
	}

	std::shared_ptr<Geometry> m {new Geometry};
	instanceCache.push_back(m);

	m->file = path;

	auto& scene = m->scene;

	scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	m->vertexBuffer.clear();
	m->uvBuffer.clear();
	m->normalBuffer.clear();
	for(uint i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh* mesh = scene->mMeshes[i];
		m->vertexBuffer.reserve(m->vertexBuffer.size()+mesh->mNumVertices);
		m->uvBuffer.reserve(m->uvBuffer.size()+mesh->mNumVertices);
		m->normalBuffer.reserve(m->normalBuffer.size()+mesh->mNumVertices);
	//	indexBuffer.reserve(indexBuffer.size()+mesh->mNumFaces*3);

		for(uint k = 0; k < mesh->mNumVertices; ++k)
		{
			m->vertexBuffer.push_back(vec3_cast<vec3>(mesh->mVertices[k]));
			if(mesh->mTextureCoords[0] && &mesh->mTextureCoords[0][k])
			{
				m->uvBuffer.push_back(vec2_cast<vec2>(mesh->mTextureCoords[0][k]));
			}
			m->normalBuffer.push_back(vec3_cast<vec3>(mesh->mNormals[k]));
		}
		for(uint k = 0; k < mesh->mNumFaces; ++k)
		{
			for(uint j = 0; j < mesh->mFaces[k].mNumIndices; ++j)
			{
				m->indexBuffer.push_back(mesh->mFaces[k].mIndices[j]);
			}
		}
	}



	aiReleaseImport(scene);

	m->Init();

	return m;
}

void BufferBase::Destroy()
{
	glDeleteBuffers(1, &id);
}

}
