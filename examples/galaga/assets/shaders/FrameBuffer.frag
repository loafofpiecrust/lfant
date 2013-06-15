#version 330 core

uniform sampler2D textureSampler;
uniform vec2 tiling;

in vec2 UV;

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(texture(textureSampler, UV).rgb, 1.0f);
}