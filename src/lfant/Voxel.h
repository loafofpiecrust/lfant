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
#pragma once

// internal
#include <lfant/Mesh.h>

// external

namespace lfant {

class Voxel : public Component
{
	DECLARE_COMP(Voxel)
public:

	class Block
	{
		friend class Voxel;
		friend class Chunk;
	public:
		Block();
		~Block();

		bool IsActive();
		void SetActive(bool value);

	protected:
		bool active = false;

	private:
	};

	class Chunk
	{
		friend class Voxel;
	public:
		Chunk();
		~Chunk();

		void Update();
		void Render();

	protected:
		static const int Size = 16;
		vector<vector<vector<Block>>> blocks;

	private:
	};

	Voxel();
	~Voxel();

	void Init();
	void Update();

	void BeginRender();
	void Render();
	void EndRender();

	void Generate();

protected:
	ptr<Shader> shader;
	ptr<Texture> texture;

	uint32_t vertexArray = 0;

	vector<Chunk> chunks;
	vector<vec3> vertexBuffer;
	uint32_t chunkId = 0;

private:

};

}