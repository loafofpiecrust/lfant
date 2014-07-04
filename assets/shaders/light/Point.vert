#version 330 core

// uniforms
uniform mat4 M;
uniform mat4 VP;
uniform float radius;

// in
layout(location=0) in vec3 position;
//layout(location=1) in vec2 UV;

// out
//out mat4 inverseVP;
//out vec2 vertUV;
//out vec3 vertPos;

void main()
{
	vec4 finalPos = VP * M * vec4(position, 1);
	gl_Position = finalPos;
//	vertPos = finalPos.xyz;
//	inverseVP = inverse(VP);
//	vertUV = UV;
}