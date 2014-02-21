/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-06-10 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

#include <lfant/Vertex.h>

namespace lfant {

void Vertex::Add(const float& value)
{
	elements.push_back({value});
}

void Vertex::Add(const vec2& value)
{
	elements.push_back({value.x, value.y});
}

void Vertex::Add(const vec3& value)
{
	elements.push_back({value.x, value.y, value.z});
}

void Vertex::Add(const vec4& value)
{
	elements.push_back({value.x, value.y, value.z, value.w});
}

void Vertex::Add(const u8vec4& value)
{
	elements.push_back({(float)value.x, (float)value.y, (float)value.z, (float)value.w});
}

const std::vector<Vertex::Element>& Vertex::Get()
{
	return elements;
}

}
