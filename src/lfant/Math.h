/******************************************************************************
 *
 *	LFANT Source
 *	Copyright (C) 2012-2013 by LazyFox Studios
 *	Created: 2012-11-04 by Taylor Snead
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

// External
#include <math.h>

// Internal

namespace lfant
{
/**	@addtogroup Engine
 *	@{
 */
/**	@addtogroup Utilities
 *	@{
 */

template<typename R = int>
inline R round(float x)
{
	if(x > 0.0f)
	{
		return floor(x + 0.5f);
	}
	else
	{
		return ceil(x - 0.5f);
	}
	//	return (x > 0.0f) ? floor(x + 0.5f) : ceil(x - 0.5f);
}

template<typename R = float>
inline R round(R x, int decimal)
{
	return floor(x * pow(10, decimal) + 0.5) / pow(10, decimal);
}

template<typename R = float>
inline R rollover(R x, R min, R max)
{
	int r = x;
	if(r >= max)
	{
		r = min;
	}
	else if(r <= min)
	{
		r = max;
	}
	return r;
}

const float pi = 3.14159265358979323846f;
const float degToRad = pi / 180;
const float radToDeg = 180 / pi;

/** @} */
/** @} */
}
