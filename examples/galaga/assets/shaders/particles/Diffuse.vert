#version 330 core

// uniforms
uniform mat4 MVP;
uniform float viewportWidth;
uniform vec3 cameraPosition;

// inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in float size;
// Will need later
//layout(location = 2) in vec3 tangent;
//layout(location = 3) in vec3 bitangent;

// outputs
out vec2 UV;

void main()
{
	// Sets the final position of the vertex.
	gl_Position = MVP * vec4(position, 1);

	gl_PointSize = size * viewportWidth / cameraPosition;
}