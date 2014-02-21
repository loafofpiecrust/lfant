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
#include <cxxabi.h>
#include <type_traits>

// Internal
#include <lfant/ptr.h>
#include <lfant/Range.h>
#include <lfant/util/String.h>

namespace lfant {

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

namespace type {

using std::enable_if;
using std::is_pointer;

string Name();

string Demangle(const string& type);
string Descope(string type, int amount = -1);
string Descope(string type, string nspace);

template<typename T>
auto Name()->typename enable_if<!is_ptr<T>::value, string>::type
{
	return type::Demangle(typeid(T).name());
}

template<typename T>
auto Name()->typename enable_if<is_ptr<T>::value, string>::type
{
	return type::Demangle(typeid(T::value_type).name());
}

template<typename T>
auto Name(T p1)->typename enable_if<!is_ptr<T>::value && !is_pointer<T>::value, string>::type
{
	return type::Demangle(typeid(p1).name());
}

template<typename T>
auto Name(T& p1)->typename enable_if<is_ptr<T>::value, string>::type
{
	return type::Demangle(typeid(*p1).name());
}

template<typename T>
auto Name(T p1) -> typename enable_if<is_pointer<T>::value, string>::type
{
	return type::Demangle(typeid(*p1).name());
}

/*
   template<typename T>
   string Type(T* p1)
   {
		return string(abi::__cxa_demangle(typeid(*p1).name(), 0, 0, (int*)0));
   }
 */

template<typename P1, typename P2, typename ... P>
string Name()
{
	return type::Name<P1>() + "," + type::Name<P2, P ...>();
}

template<typename T2, typename T1>
bool Compare(T1 p1, T2 p2)
{
	return type::Name(p1) == type::Name(p2);
}

template<typename T2, typename T1>
bool Compare(T1 p1)
{
	return type::Name(p1) == type::Name<T2>();
}

}

// Type-Casting
template<typename RT, typename IT>
RT vec2_cast(IT v)
{
	RT vv {v[0], v[1]};
	return vv;
}

template<typename RT, typename IT>
RT vec3_cast(IT v)
{
	RT vv {v[0], v[1], v[2]};
	return vv;
}

template<typename RT, typename IT>
RT vec4_cast(IT v)
{
	RT vv = {v[0], v[1], v[2], v[3]};
	return vv;
}

template<typename RT, typename IT>
RT quat_cast(IT q)
{
	RT qq {q[0], q[1], q[2], q[3]};
	return qq;
}

/** @} */
/** @} */
}
