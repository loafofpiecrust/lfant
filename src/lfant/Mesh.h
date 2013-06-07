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
*	http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/
#pragma once

#include <lfant/stdafx.h>

// Internal
#include <lfant/Material.h>
#include <lfant/Vertex.h>
#include <lfant/Renderable.h>
#include <lfant/Component.h>

// External

// System

struct aiScene;

namespace lfant
{
/** @addtogroup Game
 *	@{
 */
/** @addtogroup Rendering
 *	@{
 */

class BufferBase
{
public:
	void Destroy();

//protected:
	uint32 id = 0;
};

template<typename T>
class Buffer : public BufferBase
{
public:

	operator vector<T>&()
	{
		return data;
	}

	operator uint32()
	{
		return id;
	}

	T& operator[](uint32 i)
	{
		return data[i];
	}

	uint32 size()
	{
		return data.size();
	}

	void push_back(T t)
	{
		data.push_back(t);
	}

	vector<T> data;
	//vector<uint32_t> index;
};

/**	The Mesh class holds a 3D mesh and possibly an animated skeleton.
 *		This class controls a Mesh as defined by the 3d importing library.
 *		It handles holding a list of vertices, bones, faces, etc. It also
 *		holds a list of animations and things involving that.
 *	@todo
 *		Be sure this works fully in conjuction with the importers Mesh classes.
 */
class Mesh : public Renderable
{
	friend class Renderer;
	DECLARE_COMP(Mesh)

public:
	Mesh();
	~Mesh();

	void Load(Properties *prop);
	void Save(Properties *prop);

	void SetShape(string preset);

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	void SetInput(string path);
	void SetTexture(string name);

	void LoadFile(string path);

	ptr<Material> material = new Material;

protected:

	virtual void BeginRender();
	virtual void Render();
	virtual void EndRender();

	// Raw Rendering Functions not to be used outside.

	static uint32 CreateBuffer(void* data, uint32 size, int target, int mode = 0);

	template<typename T>
	static void CreateBuffer(Buffer<T>& data, int target, int mode = 0)
	{
		data.id = CreateBuffer(&data[0], sizeof(T)*data.size(), target, mode);
	}

	string file = "";

	uint32_t vertexArray = 0;

	uint32_t matrixId = 0;
	bool castShadows = true;
	bool receiveShadows = true;

	bool initialized = false;

	Buffer<vec3> vertexBuffer;
	Buffer<vec2> uvBuffer;
	Buffer<vec3> normalBuffer;
	Buffer<uint32> indexBuffer;

private:

	void LoadOBJ(string path);

	void IndexVBO();

	aiScene* scene = nullptr;
};

/// @}
/// @}
}
