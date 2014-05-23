/*
*	Copyright (C) 2013-2014, by loafofpiecrust
*	Created: 2013-02-11 by Taylor Snead
*
*	Licensed under the Apache License, Version 2.0 (the "License");
*	you may not use this file except in compliance with the License.
*	You may obtain a copy of the License in the accompanying LICENSE file or at
*		http://www.apache.org/licenses/LICENSE-2.0
*/

// External
#include <boost/algorithm/string.hpp>

// Internal
#include <lfant/MeshLoader.h>
#include <lfant/util/String.h>

namespace lfant
{

using boost::algorithm::to_lower;

Geometry* LoadMesh(string name)
{
	std::deque<string> tokens = Split(name, ".", "");
	string ext = tokens[tokens.size()-1];
	to_lower(ext);
	if(ext == "obj")
	{
		return LoadObj(name);
	}
	return nullptr;
}

Geometry* LoadObj(string name)
{

	return nullptr;
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
	std::map<PackedVertex, uint32>& VertexToOutIndex,
	uint32& result
	){
	std::map<PackedVertex, uint32>::iterator it = VertexToOutIndex.find(packed);
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
			  std::vector<uint32>& out_indices,
			  std::vector<glm::vec3>& out_vertices,
			  std::vector<glm::vec2>& out_uvs,
			  std::vector<glm::vec3>& out_normals)
{
	std::map<PackedVertex, uint32> VertexToOutIndex;

	// For each input vertex
	for ( uint i=0; i<in_vertices.size(); i++ )
	{
		PackedVertex packed = {in_vertices[i], in_uvs[i], in_normals[i]};

		// Try to find a similar vertex in out_XXXX
		uint32 index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found ) {        // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}
		else
		{         // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs.push_back( in_uvs[i]);
			out_normals.push_back( in_normals[i]);
			uint32 newindex = (uint32)out_vertices.size() - 1;
			out_indices.push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}
}

void IndexVBO(std::vector<glm::vec3>& in_vertices,
			  std::vector<glm::vec2>& in_uvs,
			  std::vector<uint32>& out_indices,
			  std::vector<glm::vec3>& out_vertices,
			  std::vector<glm::vec2>& out_uvs)
{
	std::map<PackedVertex, uint32> VertexToOutIndex;

	// For each input vertex
	for ( uint i=0; i<in_vertices.size(); i++ )
	{
		PackedVertex packed = {in_vertices[i], in_uvs[i]};

		// Try to find a similar vertex in out_XXXX
		uint32 index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found ) {        // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}
		else
		{         // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			out_uvs.push_back( in_uvs[i]);
			uint32 newindex = (uint32)out_vertices.size() - 1;
			out_indices.push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}
}

void IndexVBO(std::vector<glm::vec3>& in_vertices,
			  std::vector<uint32>& out_indices,
			  std::vector<glm::vec3>& out_vertices)
{
	std::map<PackedVertex, uint32> VertexToOutIndex;

	// For each input vertex
	for ( uint i=0; i<in_vertices.size(); i++ )
	{
		PackedVertex packed = {in_vertices[i]};

		// Try to find a similar vertex in out_XXXX
		uint32 index;
		bool found = getSimilarVertexIndex_fast( packed, VertexToOutIndex, index);

		if ( found ) {        // A similar vertex is already in the VBO, use it instead !
			out_indices.push_back( index );
		}
		else
		{         // If not, it needs to be added in the output data.
			out_vertices.push_back( in_vertices[i]);
			uint32 newindex = (uint32)out_vertices.size() - 1;
			out_indices.push_back( newindex );
			VertexToOutIndex[ packed ] = newindex;
		}
	}
}

}
