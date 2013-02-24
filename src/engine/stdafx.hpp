/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
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

// Detect platform if not specified
#if !WINDOWS && !LINUX && !OSX
#	ifdef _WIN32
#		define WINDOWS 1
#	elif __linux
#		define LINUX 1
#	elif _APPLE_
#		define OSX 1
#	endif
#endif

// Platform Preprocessor
#if OSX || LINUX
#	define UNIX 1
#endif

#if WINDOWS
#	define WIN32_LEAN_AND_MEAN
#	include <windows.h>
#	include <conio.h>
#	define GLFWCALL __stdcall
#elif UNIX
#	define SFEXP extern "C++"
#	define GLFWCALL
#	include <unistd.h>
#endif

// General Preprocessor
#define GLFW_DLL 1
#define null(type) *((type*)nullptr)
#define GLM_FORCE_CXX11
#define GLM_SWIZZLE
#define BOOST_EXCEPTION_DISABLE

// Includes
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <utility>
#include <iostream>
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
#include <boost/lexical_cast.hpp>

#ifdef major
#undef major
#endif

// Namespaces
using namespace std;
using namespace glm;
using namespace glm::detail;
//using namespace boost;
using namespace boost::assign;
using namespace boost::algorithm;

// Boost usings
using boost::lexical_cast;

// Typedefs
typedef uint8_t byte;
typedef uint32_t uint;

typedef tvec2<int> ivec2;
typedef tvec3<int> ivec3;
typedef tvec2<uint32_t> uvec2;
typedef tvec3<uint32_t> uvec3;

/// Unsigned char 4-component vector
typedef tvec3<unsigned char> cvec3;
typedef tvec3<unsigned char> rgb;
typedef tvec4<unsigned char> cvec4;
typedef tvec4<unsigned char> rgba;
