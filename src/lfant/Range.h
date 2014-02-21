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
 *	@{
 */
/** @addtogroup Utilities
 *	@{
 */

template<typename T = float>
class Range
{
public:
	union
	{
		T min, start, major;
	};

	union
	{
		T max, end, minor;
	};

	Range(T min, T max) :
		min(min), max(max)
	{
	}

	Range(T both) :
		min(both), max(both)
	{
	}

	Range(const Range<T>& other) :
		min(other.min), max(other.max)
	{
	}

	Range<T>& operator=(const Range<T>& other)
	{
		min = other.min;
		max = other.max;
		return *this;
	}

	T& operator[](unsigned int idx)
	{
		if(idx == 0) return min;
		return max;
	}

	Range() {}
};

/// @}
/// @}

} /* namespace lfant */
