/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2012-08-04 by Taylor Snead
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

#include <lfant/Random.hpp>

// External
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <ctime>

// Internal

namespace lfant
{

boost::random::mt19937 randfast;
boost::random::mt19937_64 randfast64;

boost::random::random_device randomizer;

void seedrand()
{
	randfast.seed(time(0));
	randfast64.seed(time(0));
}

double random(double min, double max, bool slow)
{
	boost::random::uniform_real_distribution<double> d(min, max);
	if(slow)
	{
		return d(randomizer);
	}
	return d(randfast64);
}

float random(float min, float max, bool slow)
{
	boost::random::uniform_real_distribution<float> d(min, max);
	if(slow)
	{
		return d(randomizer);
	}
	return d(randfast);
}

template<typename T>
T random(T min, T max, bool slow)
{
	boost::random::uniform_int_distribution<T> d(min, max);
	if(slow)
	{
		return d(randomizer);
	}
	if(sizeof(T) >= 8)
	{
		// If we want 64-bit, use 64-bit generator.
		return d(randfast64);
	}
	return d(randfast);
}

rgba random(rgba min, rgba max, bool slow)
{
	rgba result;
	result.r = random(min.r, max.r, slow);
	result.g = random(min.g, max.g, slow);
	result.b = random(min.b, max.b, slow);
	result.a = random(min.a, max.a, slow);
	return result;
}

vec3 random(vec3 min, vec3 max, bool slow)
{
	vec3 result;
	result.x = random(min.x, max.x, slow);
	result.y = random(min.y, max.y, slow);
	result.z = random(min.z, max.z, slow);
	return result;
}
}
