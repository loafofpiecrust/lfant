#version 330 core

// uniforms
uniform sampler2D textureSampler;

// inputs
in vec3 geomPosition;

// outputs
layout(location=0) out vec4 diffuseColor;
layout(location=1) out vec3 positionColor;
layout(location=2) out vec3 normalColor;

void main()
{
/*	vec4 color = texture2D( textureSampler, gl_TexCoord[0].st );
	if(color.a <= 0.1f)
	{
		discard;
	}
	diffuseColor = color * gl_Color;*/
	diffuseColor = gl_Color;
	positionColor = geomPosition;
	normalColor = vec3(0,0,1);
}