#version 330 core

// uniforms
uniform sampler2D textureSampler;

// inputs
//in vec4 fragColor;

// outputs
out vec4 colorOut;

void main()
{
//	colorOut = vec4(vec3(texture2D( textureSampler, gl_TexCoord[0].st ) * gl_Color), 1);
//	colorOut = vec4(gl_TexCoord[0].st, 1, 1);
	colorOut = vec4(0.0f, 0.0f, 0.5f, 0.75f);
//	colorOut = vec4(gl_Color.rgb, 1);
}