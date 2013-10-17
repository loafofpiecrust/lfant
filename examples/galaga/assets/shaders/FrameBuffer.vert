#version 330 core

layout(location = 0) in vec2 position;

out vec2 UV;

void main()
{
	gl_Position = vec4(position.xy, 1, 1);
	UV = (position.xy+vec2(1,1))/2.0;
}