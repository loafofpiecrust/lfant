#version 330 core

//uniform mat4 MVP;

layout(location = 0) in vec2 position;
//layout(location = 1) in vec2 uvin;
//layout(location = 2) in vec3 normal;

out vec2 UV;

void main()
{
	gl_Position = vec4(position.xy, 1, 1);
//	UV = uvin;
	UV = (position.xy+vec2(1,1))/2.0;
}