#version 330 core

// uniforms
uniform vec3 color;

// inputs
in vec4 vertColor;

// outputs
out vec4 finalColor;

void main()
{
	// Modify color here.
//	color = texture2D( textureSampler, UV ).rgba;
	finalColor = vec4(vertColor.rgb, 1);
}
