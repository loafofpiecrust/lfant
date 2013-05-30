#version 330 core

// uniforms
uniform sampler2D textureSampler;

// inputs
in vec2 UV;

// outputs
out vec4 color;

void main()
{
	// Modify color here.
	color = texture2D( textureSampler, UV ).rgba;
//	color = vec3(0.6, 0.2, 0);
}
