#version 330 core

//uniform sampler2D textureSampler;
//uniform vec2 tiling;
uniform sampler2D diffuseTex;
uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D lightTex;
//uniform sampler2D specularTex;
//uniform sampler2D depthTex;

// in
in vec2 UV;

// out
layout(location = 0) out vec4 color;

void main()
{
	vec4 diffuse = texture2D(diffuseTex, UV);
	vec4 position = texture2D(positionTex, UV);
	vec4 normal = texture2D(normalTex, UV);
	vec4 light = texture2D(lightTex, UV);
//	vec4 specular = texture2D(specularTex, UV);
//	vec4 depth = texture2D(depthTex, UV);

	color = light;
}