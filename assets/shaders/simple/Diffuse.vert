#version 330 core

// uniforms
uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

// inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normal;
// Will need later
//layout(location = 2) in vec3 tangent;
//layout(location = 3) in vec3 bitangent;

// outputs
//out vec2 UV;
out vec3 vertPos;
out vec3 vertNormal;

void main()
{
	vec4 pos = V * M * vec4(position, 1);
//	pos.x = -pos.x;
	// Sets the final position of the vertex.
	vertPos = pos.xyz;
	gl_Position = P * pos;
//	gl_Position.z = -gl_Position.z;

	// Sets fragment UV to vertex UV
	gl_TexCoord[0].xy = vertexUV;

	vertNormal = normalize(M * vec4(normal, 0)).xyz;
}
