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
 *		http://www.apache.org/licenses/LICENSE-2.0
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

/**
 *
 *	@details
 *	@todo
 */
template<typename T = uint32_t>
class Rect
{
public:
	typedef tvec2<T> vec_t;

	Rect()
	{
	}

	Rect(T x, T y, T width, T height) :
		x(x), y(y), width(width), height(height)
	{
	}

	Rect(vec_t pos, T width, T height) :
		Rect(pos.x, pos.y, width, height)
	{
	}

	Rect(T x, T y, vec_t size) :
		Rect(x, y, size.x, size.y)
	{
	}

	Rect(vec_t pos, vec_t size) :
		Rect(pos.x, pos.y, size.x, size.y)
	{
	}

	~Rect()
	{
	}

	T x;
	T y;
	T width;
	T height;
};

/** @} */
/** @} */
}
