/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-26 by Taylor Snead
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
#include <lfant/stdafx.h>

// External

// Internal

namespace lfant
{
/** @addtogroup Game
 *	 @{
 */
/** @addtogroup Rendering
 *	 @{
 */

// 56 bytes
// 5,952 tris/mb
// 5,952,000 tris/gb
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

/** @} */
/** @} */
}
