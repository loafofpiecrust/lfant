#version 330 core

// uniforms
//uniform sampler2D textureSampler;

// inputs
uniform vec3 color;

// outputs
out vec4 finalColor;

void main()
{
	// Modify color here.
//	color = texture2D( textureSampler, UV ).rgba;
	finalColor = vec4(color, 1);
}
