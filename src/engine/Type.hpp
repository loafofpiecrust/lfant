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
#include <cxxabi.h>
#include <type_traits>

// Internal

namespace sfs
{

	/** @addtogroup Game
	 *	 @{
	 */
	/** @addtogroup TypeInfo
	 *	 @{
	 */

	template<typename T>
	string Type()
	{
		return string(abi::__cxa_demangle(typeid(T).name(), 0, 0, (int*)0));
	}

	template<typename T>
	string Type(T ent)
	{
		return string(abi::__cxa_demangle(typeid(ent).name(), 0, 0, (int*)0));
	}

	template<typename P>
	string ParamType()
	{
		return "," + Type<P>();
	}
	template<typename P, typename P2, typename ...T>
	string ParamType()
	{
		return "," + Type<P>() + "," + Type<P2>() + ParamType<T...>();
	}

	template<typename P>
	string ParamType(P ent)
	{
		return "," + Type(ent);
	}
	template<typename P, typename P2, typename ...T>
	string ParamType(P ent, P2 ent2, T ... ents)
	{
		return "," + Type(ent) + "," + Type(ent2) + ParamType(ents...);
	}

	template<typename P, typename P2, typename ... T>
	string Type()
	{
		return Type<P>() + ParamType<P2, T...>();
	}

	template<typename P, typename P2, typename ... T>
	string Type(P ent, P2 ent2, T ... ents)
	{
		return Type(ent) + ParamType(ent2, ents...);
	}

	template<typename T, typename V>
	bool checkType(V ent, T p2)
	{
		if(!p2)
		{
			return typeid(ent) == typeid(T);
		}
		return typeid(ent) == typeid(p2);
	}

	template<typename T, typename V>
	bool checkType(V ent)
	{
		return typeid(ent) == typeid(T);
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
