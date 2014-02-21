/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
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

	operator std::vector<T>&()
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

	void pop_back()
	{
		data.pop_back();
	}

	void Create(int target, int mode = 0)
	{
		if(mode == 0)
		{
			// @todo Find actual value of GL_STATIC_DRAW
		//	mode = GL_STATIC_DRAW;
		}
		this->target = target;
	//	glGenBuffers(1, &id);
	//	glBindBuffer(target, id);
	//	glBufferData(target, sizeof(T)*size(), &data[0], mode);
	}

	void Bind()
	{
	//	glBindBuffer(target, id);
	}

	std::vector<T> data;
	int target = -1;
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

public:
	Mesh();
	~Mesh();

	virtual void Load(Properties *prop);
	virtual void Save(Properties *prop) const;

	void SetShape(string preset);

	void SetInput(string path);
	void SetTexture(string name);

	virtual void LoadFile(string path);

	static uint32 CreateBuffer(void* data, uint32 size, int target, int mode = 0);

	template<typename T>
	static void CreateBuffer(Buffer<T>& data, int target, int mode = 0)
	{
		data.id = CreateBuffer(&data[0], sizeof(T)*data.size(), target, mode);
	}

	ptr<Material> material = new Material;
	bool usingCamera = true;
	bool fboQuad = false;

protected:

	virtual void Init();
	virtual void Update();
	virtual void Render();
	virtual void Deinit();

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
