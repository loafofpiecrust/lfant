#version 330 core

// uniforms

// ins
layout(location = 0) in vec3 position;

// outs

void main()
{
	gl_Position = position;
}
