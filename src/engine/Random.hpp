/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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
#include "stdafx.hpp"

// External
//#include <boost/random.hpp>
//#include <boost/random/random_device.hpp>

// Internal

namespace sfs
{
/** @addtogroup Engine
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

void seedrand();

/// These use mt19937 or mt19937_64, when the bool is true, it uses truly random numbers (20x slower)
double random(double min, double max, bool slow = false);

float random(float min, float max, bool slow = false);

template<typename T>
T random(T min, T max);

uint32_t random(uint32_t min, uint32_t max);

uint64_t random(uint64_t min, uint64_t max);

rgba random(rgba min, rgba max, bool slow = false);

vec3 random(vec3 min, vec3 max, bool slow = false);

/** @} */
/** @} */
}
