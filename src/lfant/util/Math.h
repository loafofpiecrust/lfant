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
/**	@addtogroup Game
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
inline void rollover(R& x, R min, R max)
{
	if(x > max)
	{
		x -= max;
	}
	else if(x < min)
	{
		x += max;
	}
}

const float pi = 3.1415926535897932384626433832795f;
const float pi2 = pi*2;

/** @} */
/** @} */
}
