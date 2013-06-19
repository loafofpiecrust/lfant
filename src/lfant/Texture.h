/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-10-27 by Taylor Snead
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
#include <lfant/Object.h>

// External
#include <GL/glew.h>

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Rendering
 *	 @{
 */

/**
 *	Base class for texture handling.
 */
class Texture : public Object
{
public:
	enum class FilterMode : byte
	{
		Point, Bilinear, Trilinear
	};

	enum class WrapMode : uint16
	{
	//	Clamp = 0x812F,
	//	Repeat = 0x2901
		Clamp = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT
	};

	enum class Format : uint16
	{
		/*
		Rgb = 0x1907,
		Rgba = 0x1908,
		CompressedRgb = 0x84ED,
		CompressedRgba = 0x84EE,
		Rgb32f = 0x8815,
		Rgba32f = 0x8814
		*/
		Rgb = GL_RGB,
		Rgba = GL_RGBA,
		Rgb32f = GL_RGB32F,
		Rgba32f = GL_RGBA32F
	};

	enum class ScaleFilter : uint16
	{
	//	Nearest = 0x2600,
	//	Linear = 0x2601
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};

	Texture();

	Texture& operator=(string name)
	{
		this->path = name;
		return *this;
	}

	void InitData(byte* data);
	void OnDestroy();

	void LoadFile(string path = "", int mode = 0);
	virtual void Load(Properties* prop);
	void Save(Properties *prop);

	uint32 GetId();
	uint32 GetMode();

	uvec2 GetSize();

	void Bind();
	void Unbind();

	static Texture* GetCurrent();

	uint32 GetIndex();
	void SetIndex(uint32 idx);

	void SetFormat(Format input = Format::Rgba, Format output = Format::Rgba);

	string path = "";
	WrapMode wrapMode = WrapMode::Repeat;
	FilterMode filterMode = FilterMode::Bilinear;
	uint8 anisoLevel = 1;
//	Format format = Format::Compressed;
	Format internalFormat = Format::Rgba;
	Format format = Format::Rgba;
	uvec2 size = uvec2(0);
	vec2 tiling = vec2(1);
	uint32 index = -1;
	byte msaa = 0;
	uint32 mode;
	ScaleFilter scaleFilter = ScaleFilter::Nearest;
//	uint32 uniformId = 0;

private:
	void LoadPNG(int mode);
	void LoadJPEG(int mode);
	void LoadBMP(int mode);
	void LoadDDS(int mode);

	uint32 id = -1;
//	vector<byte> data;

	static deque<Texture*> textures;
	static Texture* current;
};

/** @} */
/** @} */
}
