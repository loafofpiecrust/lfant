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

#include <lfant/stdafx.h>

// External

// Internal

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Rendering
 *	 @{
 */

/**
 *
 */
template<typename T>
class TRect
{
public:
//	typedef float T;
	typedef tvec2<T, mediump> vec_t;

	TRect() :
		x(0), y(0), width(0), height(0)
	{
	}

	TRect(T x, T y, T width, T height) :
		x(x), y(y), width(width), height(height)
	{
	}

	TRect(vec_t pos, T width, T height) :
		TRect(pos.x, pos.y, width, height)
	{
	}

	TRect(T x, T y, vec_t size) :
		TRect(x, y, size.x, size.y)
	{
	}

	TRect(vec_t pos, vec_t size) :
		TRect(pos.x, pos.y, size.x, size.y)
	{
	}

	~TRect()
	{
	}

	union {T x, left;};
	union {T y, top;};
	union {T width, right;};
	union {T height, bottom;};
};

typedef TRect<float> Rect;
typedef TRect<int32_t> IRect;
typedef TRect<uint32_t> URect;

/** @} */
/** @} */
}
