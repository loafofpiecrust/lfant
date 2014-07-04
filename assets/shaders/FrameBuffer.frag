#version 330 core

#define PI 3.14159265f

//uniform sampler2D textureSampler;
//uniform vec2 tiling;
uniform sampler2D diffuseTex;
uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D lightTex;
//uniform sampler2D specularTex;
uniform sampler2D depthTex;
uniform vec2 textureSize;

uniform bool useDof;

uniform float focalDepth; //focal distance value in meters, but you may use autofocus option below
uniform float focalLength; //focal length in mm
uniform float fstop; //f-stop value
uniform float focus;
uniform vec2 cameraRange;
uniform float ambientLight;

// in
in vec2 UV;

// out
layout(location = 0) out vec4 outColor;

// includes
#include <DepthOfField.frag>

void main()
{
	vec4 lightedColor = vec4(0);
	if(useDof)
	{
		lightedColor = vec4(depthOfField(lightTex, textureSize, cameraRange, UV, focalDepth, focalLength, fstop, focus), 1.0);
	}
	else
	{
		lightedColor = texture2D(lightTex, UV);
	}

	vec4 diffuseColor = texture2D(diffuseTex, UV);
//	outColor = vec4(lightedColor.rgb + (diffuseColor.rgb), diffuseColor.a);
	outColor = lightedColor + (diffuseColor);
//	outColor = diffuseColor;
}
