/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

#include <lfant/stdafx.hpp>

// External

// Internal
#include <lfant/Material.hpp>
#include <lfant/Vertex.hpp>

#include <lfant/Component.hpp>

namespace lfant
{
/** @addtogroup Engine
 *	@{
 */
/** @addtogroup Rendering
 *	@{
 */

template<typename T>
class Buffer
{
public:

	/*
	operator uint32_t()
	{
		return id;
	}

	operator vector<T>&()
	{
		return data;
	}
	*/

	T& operator [](uint i)
	{
		return data[i];
	}

	uint size()
	{
		return data.size();
	}

	void push_back(T t)
	{
		data.push_back(t);
	}

	vector<T> data;
	//vector<uint32_t> index;
	uint32_t id = 0;
};

/**	The Mesh class holds a 3D mesh and possibly an animated skeleton.
 *		This class controls a Mesh as defined by the 3d importing library.
 *		It handles holding a list of vertices, bones, faces, etc. It also
 *		holds a list of animations and things involving that.
 *	@todo
 *		Be sure this works fully in conjuction with the importers Mesh classes.
 */
class Mesh : public Component
{

//	REGISTER_COMP(Mesh)

	friend class Renderer;

public:
	Mesh();
	~Mesh();

	virtual void Init();
	virtual void Update();
	virtual void OnDestroy();

	void SetInput(string path);
	void SetTexture(string name);

	void LoadFile(string path);

	Buffer<vec3> vertexBuffer;
	Buffer<vec2> uvBuffer;
	Buffer<vec3> normalBuffer;
	Buffer<uint32_t> indexBuffer;

	Material material;

protected:
	string file = "";

	//uint32_t vertexBuffer = 0;
	//uint32_t indexBuffer = 0;
	uint32_t vertexArray = 0;

	uint32_t matrixId = 0;
	bool castShadows = true;
	bool receiveShadows = true;

	bool initialized = false;

private:

	void LoadOBJ(string path);
};

/// @}
/// @}
}
