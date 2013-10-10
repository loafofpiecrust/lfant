#version 330 core

#define PI  3.14159265f


//uniform sampler2D textureSampler;
//uniform vec2 tiling;
uniform sampler2D diffuseTex;
uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D lightTex;
//uniform sampler2D specularTex;
uniform sampler2D depthTex;
uniform vec2 textureSize;

uniform float focalDepth;  //focal distance value in meters, but you may use autofocus option below
uniform float focalLength; //focal length in mm
uniform float fstop; //f-stop value
uniform vec2 cameraRange;

// in
in vec2 UV;

// out
layout(location = 0) out vec4 outColor;

// includes
#include <shaders/DepthOfField.frag>

void main()
{
	vec3 dofColor = depthOfField(textureSize, cameraRange, UV, focalDepth, focalLength, fstop);

	outColor = vec4(dofColor, 1);
}