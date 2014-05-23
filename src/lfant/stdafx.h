/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-17 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once

#include <lfant/Config.h>

// Includes
#include <stdint.h>
#include <stdlib.h>
#include <string>
#include <memory>
#include <utility>
#include <new>

//#include <glm/glm.hpp>
#include <glm/geometric.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/quaternion.hpp>

//#include <boost/assign/std/vector.hpp>
//#include <boost/assign/std/deque.hpp>
//#include <boost/function.hpp>
//#include <boost/bind.hpp>
//#include <boost/type_traits.hpp>
//#include <boost/config.hpp>
#include <boost/algorithm/string.hpp>

#ifdef major
#	undef major
#endif

// Namespaces
using namespace glm;
using std::string;
//using namespace glm::detail;
//using namespace boost;
//using namespace boost::assign;
using namespace boost::algorithm;

// Boost usings

// Typedefs
typedef uint8 byte;
typedef unsigned int uint; // Generally used in 'for' loops
