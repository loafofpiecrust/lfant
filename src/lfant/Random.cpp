/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
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

#include <lfant/Random.h>

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

double Random(double min, double max)
{
	boost::random::uniform_real_distribution<double> d(min, max);
	return d(randfast64);
}

float Random(float min, float max)
{
	boost::random::uniform_real_distribution<float> d(min, max);
	return d(randfast);
}

rgba Random(rgba min, rgba max)
{
	rgba result;
	result.r = Random(min.r, max.r);
	result.g = Random(min.g, max.g);
	result.b = Random(min.b, max.b);
	result.a = Random(min.a, max.a);
	return result;
}

vec3 Random(vec3 min, vec3 max)
{
	vec3 result;
	result.x = Random(min.x, max.x);
	result.y = Random(min.y, max.y);
	result.z = Random(min.z, max.z);
	return result;
}
}
