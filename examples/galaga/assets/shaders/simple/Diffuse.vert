#version 330 core

// uniforms
uniform mat4 MVP;

// inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 normal;
// Will need later
//layout(location = 2) in vec3 tangent;
//layout(location = 3) in vec3 bitangent;

// outputs
out vec2 UV;

void main()
{
	// Sets the final position of the vertex.
	gl_Position = MVP * vec4(position, 1);

	// Sets fragment UV to vertex UV
	UV = vertexUV;
}
