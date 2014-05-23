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

// Internal
#include <lfant/Mesh.h>

namespace lfant
{

Geometry *LoadMesh(string name);

// Specific Types
Geometry* LoadObj(string name);

void IndexVBO(std::vector<glm::vec3>& in_vertices,
			  std::vector<glm::vec2>& in_uvs,
			  std::vector<glm::vec3>& in_normals,
			  std::vector<uint32>& out_indices,
			  std::vector<glm::vec3>& out_vertices,
			  std::vector<glm::vec2>& out_uvs,
			  std::vector<glm::vec3>& out_normals);

void IndexVBO(std::vector<glm::vec3>& in_vertices,
			  std::vector<glm::vec2>& in_uvs,
			  std::vector<uint32>& out_indices,
			  std::vector<glm::vec3>& out_vertices,
			  std::vector<glm::vec2>& out_uvs);

void IndexVBO(std::vector<glm::vec3>& in_vertices,
			  std::vector<uint32>& out_indices,
			  std::vector<glm::vec3>& out_vertices);

}
