/******************************************************************************
 *
 *	ShadowFox Engine Source
 *	Copyright (C) 2012-2013 by ShadowFox Studios
 *	Created: 2013-02-11 by Taylor Snead
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
#include "stdafx.hpp"

// External

// Internal
#include "Mesh.hpp"

namespace sfs
{

Mesh LoadMesh(string name);

// Specific Types
Mesh LoadObj(string name);

void IndexVBO(std::vector<glm::vec3> & in_vertices,
			  std::vector<glm::vec2> & in_uvs,
			  std::vector<glm::vec3> & in_normals,
			  std::vector<uint32_t> & out_indices,
			  std::vector<glm::vec3> & out_vertices,
			  std::vector<glm::vec2> & out_uvs,
			  std::vector<glm::vec3> & out_normals);

void IndexVBO(std::vector<glm::vec3> & in_vertices,
			  std::vector<glm::vec2> & in_uvs,
			  std::vector<uint32_t> & out_indices,
			  std::vector<glm::vec3> & out_vertices,
			  std::vector<glm::vec2> & out_uvs);

void IndexVBO(std::vector<glm::vec3> & in_vertices,
			  std::vector<uint32_t> & out_indices,
			  std::vector<glm::vec3> & out_vertices);

}
