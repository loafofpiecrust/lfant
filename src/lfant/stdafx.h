/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
*	Unless required by applicable law or agreed to in writing, software
*	distributed under the License is distributed on an "AS IS" BASIS,
*	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*	See the License for the specific language governing permissions and
*	limitations under the License.
*
******************************************************************************/
#pragma once

#include <lfant/Config.h>

// Includes
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <memory>
#include <utility>
#include <new>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/core/func_geometric.hpp>
#include <glm/core/func_common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <boost/assign/std/vector.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/type_traits.hpp>
#include <boost/config.hpp>
#include <boost/algorithm/string.hpp>

#ifdef major
#	undef major
#endif

// Namespaces
using namespace std;
using namespace glm;
using namespace glm::detail;
//using namespace boost;
using namespace boost::assign;
using namespace boost::algorithm;

// Boost usings

// Typedefs
typedef uint8_t byte;
typedef unsigned int uint; // Generally used in 'for' loops

// Int primitives, ridding suffix '_t', cuz I don't like it
typedef int8_t int8;
typedef uint8_t uint8;
typedef int16_t int16;
typedef uint16_t uint16;
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;

typedef tvec2<int> ivec2;
typedef tvec3<int> ivec3;
typedef tvec2<uint32_t> uvec2;
typedef tvec3<uint32_t> uvec3;

/// Unsigned char 4-component vector
typedef tvec3<unsigned char> rgb;
typedef tvec4<unsigned char> rgba;
