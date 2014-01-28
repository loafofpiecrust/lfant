#version 330 core

layout(points) in;
layout(triangle_strip, max_vertices = 14) out;

// uniforms
uniform mat4 MVP;

// ins

// outs

void main()
{
	vec3 pos = gl_in[0].gl_Position.xyz;

	gl_Position = MVP * vec4(pos+vec3(-1, 1,-1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3(-1,-1,-1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3( 1,-1,-1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3( 1, 1,-1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3( 1,-1, 1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3( 1, 1, 1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3(-1, 1,-1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3(-1, 1, 1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3( 1,-1, 1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3(-1,-1, 1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3(-1, 1,-1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3(-1,-1,-1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3( 1,-1, 1), 1);
	EmitVertex();

	gl_Position = MVP * vec4(pos+vec3( 1,-1,-1), 1);
	EmitVertex();
}
