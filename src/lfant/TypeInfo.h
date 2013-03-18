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
#include <cxxabi.h>
#include <type_traits>

// Internal
#include <lfant/ptr.h>
#include <lfant/Range.h>
#include <lfant/util/String.h>

namespace lfant
{

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Utilities
 *	 @{
 */

template<typename T>
struct remove_ref
{
	typedef T type;
};

template<typename T>
struct remove_ref<T&>
{
	typedef T type;
};

template<typename T>
struct remove_ref < T&& >
{
	typedef T type;
};

string Type();

string DemangleType(string type);

template<typename T>
auto Type()->typename is_not_ptr<T, string>::type
{
	return DemangleType(typeid(T).name());
}

template<typename T>
auto Type()->typename is_ptr<T, string>::type
{
	return DemangleType(typeid(T::value_type).name());
}

template<typename T>
auto Type(T p1)->typename is_not_ptr<T, string>::type
{
	return DemangleType(typeid(p1).name());
}

template<typename T>
auto Type(T& p1)->typename is_ptr<T, string>::type
{
	return DemangleType(typeid(*(p1.get())).name());
}

template<typename T>
string Type(T* p1)
{
	return DemangleType(typeid(*p1).name());
}

/*
   template<typename T>
   string Type(T* p1)
   {
		return string(abi::__cxa_demangle(typeid(*p1).name(), 0, 0, (int*)0));
   }
 */

template<typename P1, typename P2, typename ... P>
string Type()
{
	return Type<P1>() + "," + Type<P2, P ...>();
}

template<typename T2, typename T1>
bool CheckType(T1 p1, T2 p2)
{
	return Type(p1) == Type(p2);
}

template<typename T2, typename T1>
bool CheckType(T1 p1)
{
	return Type(p1) == Type<T2>();
}

// Type-Casting
template<typename RT = vec2, typename IT>
RT vec2_cast(IT v)
{
	RT vv(v[0], v[1]);
	return vv;
}

template<typename RT = vec3, typename IT>
RT vec3_cast(IT v)
{
	RT vv(v[0], v[1], v[2]);
	return vv;
}

template<typename RT = vec4, typename IT>
RT vec4_cast(IT v)
{
	RT vv(v[0], v[1], v[2], v[3]);
	return vv;
}

template<typename RT = quat, typename IT>
RT quat_cast(IT q)
{
	RT qq(q[0], q[1], q[2], q[3]);
	return qq;
}

/** @} */
/** @} */
}
