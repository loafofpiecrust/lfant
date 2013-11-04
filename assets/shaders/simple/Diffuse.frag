#version 330 core

// uniforms
uniform sampler2D textureSampler;
uniform vec2 tiling;

// inputs
//in vec2 UV;
in vec3 vertPos;
in vec3 vertNormal;

// outputs
layout(location = 1) out vec4 diffColor;
layout(location = 2) out vec3 posColor;
layout(location = 3) out vec3 normColor;
//layout(location = 3) out vec4 lightColor;
//layout(location = 4) out vec4 specularColor;

void main()
{
	// Modify color here.
	vec4 tcolor = texture2D( textureSampler, gl_TexCoord[0].xy /* tiling*/ );
//	vec4 tcolor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	if(tcolor.a == 0.0f)
	{
		discard;
	}

	posColor = vertPos;
	normColor = vertNormal;
	diffColor = tcolor;
//	lightColor = vec4(0,0,0,1);
//	specularColor = vec4(0,0,0,1);
}
