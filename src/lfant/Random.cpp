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
namespace random
{

boost::random::mt19937 randfast;
boost::random::mt19937_64 randfast64;

boost::random::random_device randomizer;

namespace
{
__attribute__((constructor))
void seedrand()
{
	printf("Seeding randoms.\n");
	randfast.seed(time(0));
	randfast64.seed(time(0));
}
}

double Range(double min, double max)
{
	boost::random::uniform_real_distribution<double> d(min, max);
	return d(randfast64);
}

float Range(float min, float max)
{
	boost::random::uniform_real_distribution<float> d(min, max);
	return d(randfast);
}

rgba Range(rgba min, rgba max)
{
	rgba result;
	result.r = Range(min.r, max.r);
	result.g = Range(min.g, max.g);
	result.b = Range(min.b, max.b);
	result.a = Range(min.a, max.a);
	return result;
}

vec3 Range(vec3 min, vec3 max)
{
	vec3 result;
	result.x = Range(min.x, max.x);
	result.y = Range(min.y, max.y);
	result.z = Range(min.z, max.z);
	return result;
}

}
}
