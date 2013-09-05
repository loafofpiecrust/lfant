#version 330 core

//uniform sampler2D textureSampler;
//uniform vec2 tiling;
uniform sampler2D diffuseTex;
uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D lightTex;
//uniform sampler2D specularTex;
//uniform sampler2D depthTex;

uniform vec3 cameraPosition;
uniform vec2 cameraDof;

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

	float dist = length(cameraPosition - position.xyz);
	if(dist > cameraDof.x-cameraDof.y && dist < cameraDof.x-cameraDof.y)
	{
		color = diffuse;
		return;
	}
/*
	vec2 blur = vec2(1.0/300.0, 1.0/200.0);
	int samp = 3;
	const float sampDiv = pow(samp-1, 4);
	vec4 blurred = vec4(0);
	for(int x = -samp; x <= samp; ++x)
	{
		for(int y = -samp; y <= samp; ++y)
		{
			blurred += texture2D(diffuseTex, vec2(UV.x + x * blur.x, UV.y + y * blur.y)) / sampDiv;
		}
	}
	// @todo Make lighting seperate to have a final addition step.
	color = blurred;
*/
	color = diffuse;
}