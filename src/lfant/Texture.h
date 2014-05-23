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
#include "gui/Image.h"

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
class Texture //: public Object
{
public:
	enum class FilterMode : byte
	{
		Point, Bilinear, Trilinear
	};

	enum class WrapMode : uint
	{
		Clamp = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT
	//	Clamp = GL_CLAMP_TO_EDGE,
	//	Repeat = GL_REPEAT
	};

	enum class Format : uint
	{
	/*	RGB = 0x1907,
		RGBA = 0x1908,
		RGBA16F = 0x881A,
		RGB32F = 0x8815,
		RGBA32F = 0x8814,

		CompressedRGB = 0x84ED,
		CompressedRGBA = 0x84EE,

		Depth = 0x1902,
		Depth24 = 0x81A6,
		Depth32F = 0x8CAC
	*/
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		RGBA16F = GL_RGBA16F,
		RGB32F = GL_RGB32F,
		RGBA32F = GL_RGBA32F,

		CompressedRGB = GL_COMPRESSED_RGB,
		CompressedRGBA = GL_COMPRESSED_RGBA,

		Depth = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL_EXT,
		Depth24 = GL_DEPTH_COMPONENT24,
		Depth32F = GL_DEPTH_COMPONENT32F,

		Depth32F_Stencil8 = GL_DEPTH32F_STENCIL8,
		Depth24_Stencil8 = GL_DEPTH24_STENCIL8_EXT,

	//	Rgb = GL_RGB,
	//	Rgba = GL_RGBA,
	//	Rgb32f = GL_RGB32F,
	//	Rgba32f = GL_RGBA32F
	};

	enum class ScaleFilter : uint
	{
		/*
		Nearest = 0x2600,
		Linear = 0x2601
		*/
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};

	enum class DataType : uint
	{
		/*
		Byte = 0x1401,
		Float = 0x1406
		*/
		Byte = GL_UNSIGNED_BYTE,
		Float = GL_FLOAT,
		Uint24_8 = GL_UNSIGNED_INT_24_8_EXT,
	};

	Texture();
	virtual ~Texture();

	Texture& operator=(string name)
	{
		this->path = name;
		return *this;
	}

	void InitData(const uint8* data);
	void InitData(const uint8* data, ivec2 offset);
	void Destroy();

	static std::shared_ptr<Texture> LoadFile(string path, int mode = -1);
	static std::shared_ptr<Texture> Load(Properties* prop);
	virtual void Save(Properties* prop) const;

	void LoadFromImage(gui::Image* image);

	/// @note What does this do...?
	void SetSmooth(bool smooth);

	uint32 GetId();
	int GetMode();

	uvec2 GetSize();

	void Bind();
	void Unbind();

	static Texture* GetCurrent();

	int GetIndex();
	void SetIndex(int idx);

	void SetFormat(Format input = Format::RGBA, Format output = Format::RGBA);

	string path = "";
	WrapMode wrapMode = WrapMode::Clamp;
	FilterMode filterMode = FilterMode::Bilinear;
	uint8 anisoLevel = 1;
//	Format format = Format::Compressed;
	Format internalFormat = Format::RGBA;
	Format format = Format::RGBA;
	DataType dataType = DataType::Byte;
	uvec2 size = uvec2(0);
//	vec2 tiling = vec2(1);
	int index = 0;
	uint8 msaa = 0;
	int mode;
	ScaleFilter scaleFilter = ScaleFilter::Nearest;
//	uint32 uniformId = 0;

private:
	void LoadPNG(string path);
	void LoadJPEG(string path);
	void LoadBMP(string path);
	void LoadDDS(string path);

	uint32 id = 0;
//	vector<byte> data;

	static std::deque<std::weak_ptr<Texture>> textureCache;
	static Texture* current;
};

/** @} */
/** @} */
}
