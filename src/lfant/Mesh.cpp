/******************************************************************************
*
*	LFANT Source
*	Copyright (C) 2012-2013 by LazyFox Studios
*	Created: 2012-07-19 by Taylor Snead
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

#include <lfant/Mesh.h>

// External
#include <GL/glew.h>

// Internal
#include <lfant/util/String.h>
#include <lfant/Transform.h>
#include <lfant/Console.h>
#include <lfant/Renderer.h>
#include <lfant/MeshLoader.h>
#include <lfant/Scene.h>
#include <lfant/Camera.h>

namespace lfant
{

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Load(Properties *props)
{
	Component::Load(props);

	if(Properties* pmat = props->GetChild("material"))
	{
		material.Load(pmat);
	}
	else
	{
		Log("Loading material from file");
		material.LoadFile(props->Get<string>("material", "materials/Diffuse.mat"));
	}
}

void Mesh::Init()
{
	Renderable::Init();
}

void Mesh::Update()
{
	Renderable::Update();
}

void Mesh::OnDestroy()
{
	Renderable::OnDestroy();
}

void Mesh::BeginRender()
{
	if(loaded)
	{
		EndRender();
	}

	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	if(material.shader.id == 0)
	{
		material.shader.LoadFile();
	}

	if(material.texture.id == 0)
	{
		material.texture.LoadFile();
	}

	if(material.shader.id != 0)
	{
		// Get any uniforms here
		matrixId = material.shader.GetUniform("MVP");
		material.texture.uniformId = material.shader.GetUniform("textureSampler");
	}

	vector<uint32_t> indices;
	vector<vec3> indexed_vertices;
	vector<vec2> indexed_uvs;
	vector<vec3> indexed_normals;
	IndexVBO(vertexBuffer.data, uvBuffer.data, normalBuffer.data, indices, indexed_vertices, indexed_uvs, indexed_normals);

	vertexBuffer.data = indexed_vertices;
	uvBuffer.data = indexed_uvs;
	normalBuffer.data = indexed_normals;
	indexBuffer.data = indices;

	vertexBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, vertexBuffer.data);
	uvBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, uvBuffer.data);
	normalBuffer.id = CreateBuffer(GL_ARRAY_BUFFER, normalBuffer.data);
	indexBuffer.id = CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.data);

	loaded = true;
}

void Mesh::Render()
{
	if(material.shader.id == 0 || material.texture.id == 0)
	{
		return;
	}

	glBindVertexArray(vertexArray);

	glUseProgram(material.shader.id);

	mat4 mvp = game->scene->mainCamera->projection * game->scene->mainCamera->view * owner->GetComponent<Transform>()->GetMatrix();
	glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, material.texture.id);
	glUniform1i(material.texture.uniformId, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.id);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer.id);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer.id);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.id);
	glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
}

void Mesh::EndRender()
{
	glDeleteBuffers(1, &vertexBuffer.id);
	glDeleteBuffers(1, &uvBuffer.id);
	glDeleteBuffers(1, &normalBuffer.id);
	glDeleteBuffers(1, &indexBuffer.id);
	glDeleteTextures(1, &material.texture.id);
	glDeleteVertexArrays(1, &vertexArray);
	loaded = false;
}

uint32_t Mesh::CreateBuffer(int target, void* data, uint32_t size, int mode)
{
	if(mode == 0)
	{
		mode = GL_STATIC_DRAW;
	}
	uint32_t id;
	glGenBuffers(1, &id);
	glBindBuffer(target, id);
	glBufferData(target, size, data, mode);
	return id;
}

void Mesh::LoadFile(string path)
{
	string ext = Extension(path);
	if(ext == "obj")
	{
		LoadOBJ(path);
	}
	else
	{
		Log("Mesh::LoadFile: File type not supported.");
		return;
	}
	BeginRender();
}

void Mesh::LoadOBJ(string path)
{
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec2> out_uvs;
	std::vector<glm::vec3> out_normals;
	std::vector<uint32_t> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE* file = fopen(path.c_str(), "r");
	if( file == NULL )
	{
		Log("Impossible to open the file! Are you in the right path?");
		return;
	}

	while( 1 ) {

		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;             // EOF = End Of File. Quit the loop.

		// else : parse lineHeader

		if ( strcmp( lineHeader, "v" ) == 0 )
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
			temp_vertices.push_back(vertex);
		}
		else if ( strcmp( lineHeader, "vt" ) == 0 )
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y );
			uv.y = -uv.y;             // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
			temp_uvs.push_back(uv);
		}
		else if ( strcmp( lineHeader, "vn" ) == 0 )
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
			temp_normals.push_back(normal);
		}
		else if ( strcmp( lineHeader, "f" ) == 0 )
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
			if (matches != 9)
			{
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return;
			}
			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
		else
		{
			// Probably a comment, eat up the rest of the line
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	for( unsigned int i=0; i<vertexIndices.size(); i++ ) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
		glm::vec2 uv = temp_uvs[ uvIndex-1 ];
		glm::vec3 normal = temp_normals[ normalIndex-1 ];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	vertexBuffer.data = out_vertices;
	uvBuffer.data = out_uvs;
	normalBuffer.data = out_normals;
	//indexBuffer.data = vertexIndices;
}

}
