#version 330 core

// uniforms
uniform sampler2D textureSampler;
uniform vec2 tiling;

// inputs
in vec2 UV;

// outputs
layout(location = 0) out vec4 color;

void main()
{
	// Modify color here.
	vec4 tcolor = vec4(texture2D( textureSampler, UV/*tiling*/ ).rgb, 1);
//	vec4 tcolor = vec4(texelFetch( textureSampler, UV/*tiling*/ ).rgb, 1);
//	vec4 tcolor = vec4(1,0,1,1);
//	if(tcolor.a == 0.0f)
	{
//		discard;
	}

	color = tcolor;
//	gl_FragColor = tcolor;
}
