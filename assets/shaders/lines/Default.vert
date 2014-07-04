#version 330 core

// uniforms
uniform mat4 matrix;

// inputs
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
// Will need later
//layout(location = 2) in vec3 tangent;
//layout(location = 3) in vec3 bitangent;

// outputs
out vec4 vertColor;

void main()
{
	// Sets the final position of the vertex.
	gl_Position = matrix * vec4(position.xyz, 1);
	vertColor = color;
}
