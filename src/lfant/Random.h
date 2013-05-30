/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-28 by Taylor Snead
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
#include <boost/random.hpp>
//#include <boost/random/random_device.hpp>

// Internal

namespace lfant
{
namespace random
{

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

extern boost::random::mt19937 randfast;
extern boost::random::mt19937_64 randfast64;

/// These use mt19937 or mt19937_64, when the bool is true, it uses truly random numbers (20x slower)

template<typename T>
T Range(T min, T max)
{
	boost::random::uniform_int_distribution<T> d(min, max);
	if(sizeof(T) >= 8)
	{
		return d(randfast64);
	}
	return d(randfast);
}

float Range(float min, float max);
double Range(double min, double max);
rgba Range(rgba min, rgba max);
vec3 Range(vec3 min, vec3 max);

/** @} */
/** @} */

}
}
