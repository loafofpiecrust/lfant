#version 330 core

// uniforms
uniform sampler2D textureSampler;

// outputs
out vec4 color;

void main()
{
	color = texture2D( textureSampler, gl_TexCoord[0] ).rgba;
}