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
#include <glm/geometric.hpp>
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
u8vec4 Range(u8vec4 min, u8vec4 max);
vec3 Range(vec3 min, vec3 max);
vec4 Range(vec4 min, vec4 max);

/** @} */
/** @} */

}
}
