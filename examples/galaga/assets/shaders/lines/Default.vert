#version 330 core

// uniforms
uniform mat4 matrix;

// inputs
layout(location = 0) in vec3 position;
// Will need later
//layout(location = 2) in vec3 tangent;
//layout(location = 3) in vec3 bitangent;

// outputs

void main()
{
	// Sets the final position of the vertex.
	gl_Position = matrix * vec4(position, 1);
}
