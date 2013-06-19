#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 24) out;

// uniforms
uniform mat4 MVP;

// ins

// outs

void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;

	gl_Position = MVP * (pos + vec3());
}