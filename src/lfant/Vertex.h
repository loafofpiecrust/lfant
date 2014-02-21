/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2012-07-26 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/
#pragma once
#include <lfant/stdafx.h>

// External
#include <vector>

// Internal

namespace lfant {

/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Rendering
 *	 @{
 */

// 56 bytes
// 5,952 tris/mb
// 5,952,000 tris/gb
 /*
struct Vertex
{
	vec3 vert;
	vec2 tex;
	vec3 normal;
	//vec3 tangent;
	//vec3 bitangent;
	//byte padding[4];

	Vertex(vec3 vert = vec3(0), vec2 tex = vec2(0), vec3 normal = vec3(0, 0, 1)) :
		vert(vert), tex(tex), normal(normal)
	{
	}
	//Vertex(vec3 vert = vec3(0)) : vert(vert)
	//{
	//}
};
*/

class Vertex
{
public:
	typedef std::vector<float> Element;

	void Add(const float& value);
	void Add(const vec2& value);
	void Add(const vec3& value);
	void Add(const vec4& value);
	void Add(const u8vec4& value);

	const std::vector<Element>& Get();

protected:
	std::vector<Element> elements;
};

/** @} */
/** @} */
}
