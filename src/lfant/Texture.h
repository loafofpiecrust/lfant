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
#include <lfant/Object.h>

// External
//#include <GL/glew.h>

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
		Clamp = 0x812F,
		Repeat = 0x2901
	//	Clamp = GL_CLAMP_TO_EDGE,
	//	Repeat = GL_REPEAT
	};

	enum class Format : uint16
	{
		RGB = 0x1907,
		RGBA = 0x1908,
		RGBA16F = 0x881A,
		RGB32F = 0x8815,
		RGBA32F = 0x8814,

		CompressedRGB = 0x84ED,
		CompressedRGBA = 0x84EE,

		Depth = 0x1902,
		Depth24 = 0x81A6,
		Depth32F = 0x8CAC

	//	Rgb = GL_RGB,
	//	Rgba = GL_RGBA,
	//	Rgb32f = GL_RGB32F,
	//	Rgba32f = GL_RGBA32F
	};

	enum class ScaleFilter : uint16
	{
		Nearest = 0x2600,
		Linear = 0x2601
	//	Nearest = GL_NEAREST,
	//	Linear = GL_LINEAR
	};

	enum class DataType : uint16
	{
		Byte = 0x1401,
		Float = 0x1406
	};

	Texture();

	Texture& operator=(string name)
	{
		this->path = name;
		return *this;
	}

	void InitData(byte* data);
	void Deinit();

	void LoadFile(string path = "", int mode = -1);
	virtual void Load(Properties* prop);
	virtual void Save(Properties* prop) const;

	uint32 GetId();
	uint32 GetMode();

	uvec2 GetSize();

	void Bind();
	void Unbind();

	static Texture* GetCurrent();

	uint32 GetIndex();
	void SetIndex(uint32_t idx);

	void SetFormat(Format input = Format::RGBA, Format output = Format::RGBA);

	string path = "";
	WrapMode wrapMode = WrapMode::Repeat;
	FilterMode filterMode = FilterMode::Bilinear;
	uint8 anisoLevel = 1;
//	Format format = Format::Compressed;
	Format internalFormat = Format::RGBA;
	Format format = Format::RGBA;
	DataType dataType = DataType::Byte;
	uvec2 size = uvec2(0);
	vec2 tiling = vec2(1);
	uint32 index = 0;
	uint8 msaa = 0;
	uint32 mode;
	ScaleFilter scaleFilter = ScaleFilter::Nearest;
//	uint32 uniformId = 0;

private:
	void LoadPNG(string path);
	void LoadJPEG(string path);
	void LoadBMP(string path);
	void LoadDDS(string path);

	uint32 id = -1;
//	vector<byte> data;

	static std::deque<Texture*> textures;
	static Texture* current;
};

/** @} */
/** @} */
}
