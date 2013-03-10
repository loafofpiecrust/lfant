/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
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

// External

// Internal
#include <lfant/MeshLoader.h>
#include <lfant/String.h>

namespace lfant
{

Mesh LoadMesh(string name)
{
	vector<string> tokens = Split(name, ".", "");
	string ext = tokens[tokens.size()-1];
	to_lower(ext);
	if(ext == "obj")
	{
		return LoadObj(name);
	}
}

Mesh LoadObj(string name)
{


}

struct PackedVertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex that) const {
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	}
};

bool getSimilarVertexIndex_fast(
    PackedVertex& packed,
    std::map<PackedVertex, uint32_t>& VertexToOutIndex,
    uint32_t& result
    ){
	std::map<PackedVertex, uint32_t>::iterator it = VertexToOutIndex.find(packed);
	if ( it == VertexToOutIndex.end() ) {
		return false;
	}else{
		result = it->second;
		return true;
	}
}

void IndexVBO(std::vector<glm::vec3>& in_vertices,
              std::vector<glm::vec2>& in_uvs,
              std::vector<glm::vec3>& in_normals,
              std::vector<uint32_t>& out_indices,
              std::vector<glm::vec3>& out_vertices,
              std::vector<glm::vec2>& out_uvs,
              std::vector<glm::vec3>& out_normals)
{
	std::map<PackedVertex, uint32_t> VertexToOutIndex;

	// For each input vertex
	for ( uint i=0; i<in_vertices.size(); i++ )
	{
		PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};

		// Try to find a similar vertex in out_XXXX
		uint32_t index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found ) {        // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}
		else
		{         // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs.push_back( in_uvs[i]);
			out_normals.push_back( in_normals[i]);
			uint32_t newindex = (uint32_t)out_vertices.size() - 1;
			out_indices.push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}
}

void IndexVBO(std::vector<glm::vec3>& in_vertices,
              std::vector<glm::vec2>& in_uvs,
              std::vector<uint32_t>& out_indices,
              std::vector<glm::vec3>& out_vertices,
              std::vector<glm::vec2>& out_uvs)
{
	std::map<PackedVertex, uint32_t> VertexToOutIndex;

	// For each input vertex
	for ( uint i=0; i<in_vertices.size(); i++ )
	{
		PackedVertex packed = {in_vertices[i], in_uvs[i]};

		// Try to find a similar vertex in out_XXXX
		uint32_t index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found ) {        // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}
		else
		{         // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs.push_back( in_uvs[i]);
			uint32_t newindex = (uint32_t)out_vertices.size() - 1;
			out_indices.push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}
}

void IndexVBO(std::vector<glm::vec3>& in_vertices,
              std::vector<uint32_t>& out_indices,
              std::vector<glm::vec3>& out_vertices)
{
	std::map<PackedVertex, uint32_t> VertexToOutIndex;

	// For each input vertex
	for ( uint i=0; i<in_vertices.size(); i++ )
	{
		PackedVertex packed = {in_vertices[i]};

		// Try to find a similar vertex in out_XXXX
		uint32_t index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found ) {        // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}
		else
		{         // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			uint32_t newindex = (uint32_t)out_vertices.size() - 1;
			out_indices.push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}
}

}
