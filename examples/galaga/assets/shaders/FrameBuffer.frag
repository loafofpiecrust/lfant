#version 330 core

uniform sampler2D textureSampler;
uniform vec2 tiling;

in vec2 UV;

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(texture2D(textureSampler, UV).rgb, 1);
//	color = vec4(0.0f, 1.0f, 0.0f, 1.0f);
}