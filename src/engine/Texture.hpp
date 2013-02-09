/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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
#include "stdafx.hpp"

// External

// Internal

namespace sfs
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Rendering
 *	 @{
 */

/**	Base class for texture handling.
 *	@details
 *	@todo
 */
class Texture
{
public:

	enum FilterMode
	{
		FM_POINT, FM_BILINEAR, FM_TRILINEAR
	};

	enum WrapMode
	{
		WM_CLAMP, WM_REPEAT
	};

	enum TexFormat
	{
		TF_COMPRESSED, TF_16BIT, TF_TRUECOLOR
	};

	string name = "";
	WrapMode wrapMode = WM_REPEAT;
	FilterMode filterMode = FM_BILINEAR;
	uint16_t anisoLevel = 1;
	TexFormat format = TF_COMPRESSED;
	uint16_t width;
	uint16_t height;
	uint32_t id;
	uint32_t uniformId;

	operator uint32_t()
	{
		return id;
	}

	operator string()
	{
		return name;
	}

	void operator=(uint32_t texId)
	{
		id = texId;
	}

	void operator=(string file);

	Texture()
	{
	}

	Texture(string file, uint16_t width, uint16_t height, uint32_t id) :
		name(file), width(width), height(height), id(id)
	{
	}
};

/** @} */
/** @} */
}
