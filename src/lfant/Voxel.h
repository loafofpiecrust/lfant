/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-06-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

// internal
#include <lfant/Mesh.h>

// external

namespace lfant {

class Voxel : public Component
{
public:

	class Block
	{
		friend class Voxel;
		friend class Chunk;
	public:

		enum class Type : byte
		{
			Default = 0,

			Grass,
			Dirt,

			Count
		};

		Block();
		~Block();

		bool IsActive();
		void SetActive(bool value);

	protected:
		bool active = false;
		Type type = Type::Default;

	private:
	};

	class Chunk
	{
		friend class Voxel;
	public:
		Chunk();
		~Chunk();

		void Update();

		void BeginRender();
		void Render();
		void EndRender();

	protected:
		static const int Size = 16;
		std::vector<std::vector<std::vector<Block>>> blocks;
		Buffer<vec3> blockBuffer;

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

	std::vector<std::vector<std::vector<Chunk>>> chunks;

private:

};

}
