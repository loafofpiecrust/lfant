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
	typedef glm::detail::tvec2<T, glm::mediump> vec_t;

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
typedef TRect<int32> IRect;
typedef TRect<uint32> URect;

/** @} */
/** @} */
}
