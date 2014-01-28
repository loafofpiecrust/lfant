#version 330 core
#extension GL_EXT_geometry_shader4 : enable

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// uniform
//uniform mat4 mvp;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPosition;

// input
in vec4 vertColor[];
//in vec3 vertPosition[];
in float vertSize[];

// output
//out vec4 fragColor;

void main()
{
	vec3 normal = gl_in[0].gl_Position.xyz - cameraPosition;
	normal = vec4(view * vec4(normal,1)).xyz;

	float size = /*vertSize[0]*/0.5f;
	vec3 right = normalize(cross(normal, vec3(0,1,0)))*size;
	vec3 up = normalize(cross(right, normal))*size;
	vec3 pos = vec4(view * gl_in[0].gl_Position).xyz;

	gl_Position =  vec4(pos - right - up, 1);
	gl_Position = projection * gl_Position;
	gl_TexCoord[0].st = vec2(0,0);
	gl_FrontColor = vertColor[0];
	EmitVertex();

	gl_Position =  vec4(pos + right - up, 1);
	gl_Position.x += size;
	gl_Position = projection * gl_Position;
	gl_TexCoord[0].st = vec2(1,0);
	gl_FrontColor = vertColor[0];
	EmitVertex();

	gl_Position = vec4(pos - right + up, 1);
	gl_Position.y += size;
	gl_Position = projection * gl_Position;
	gl_TexCoord[0].st = vec2(0,1);
	gl_FrontColor = vertColor[0];
	EmitVertex();

	gl_Position =  vec4(pos + right + up, 1);
	gl_Position.xy += vec2(size);
	gl_Position = projection * gl_Position;
	gl_TexCoord[0].st = vec2(1,1);
	gl_FrontColor = vertColor[0];
	EmitVertex();

	EndPrimitive();
}