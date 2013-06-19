#version 330 core
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

// uniform
uniform mat4 MVP;
uniform vec3 cameraPosition;

// input
in vec4 vertColor[];
in vec3 vertPosition[];
in float vertSize[];

// output
out vec4 fragColor;

void main()
{
//	vec3 pos = vertPosition[0];
	vec4 pos = vec4(vertPosition[0], 1);
//	vec3 toCamera = normalize(cameraPosition - pos);
//	vec3 right = cross(toCamera, vec3(0,1,0));
	float size = 0.1;
	vec4 trans = /*MVP */ normalize(pos);
	trans.z = 0.5;
//	vec4 pos = vec4(0.5, 0.5, 0, 1);

	// Vert 1
//	gl_TexCoord[0].st = vec2(1,1);
//	gl_Position = MVP * vec4(pos + vec3(size, size,0), 1);
	gl_Position = trans + vec4(size, size,0,1);
	fragColor = vertColor[0];
	EmitVertex();

	// Vert 2
//	gl_TexCoord[0].st = vec2(1,-1);
//	gl_Position = MVP * vec4(pos + vec3(size, -size,0), 1);
	gl_Position = trans + vec4(size, -size,0,1);
	fragColor = vertColor[0];
	EmitVertex();

	// Vert 3
//	gl_TexCoord[0].st = vec2(-1,1);
//	gl_Position = MVP * vec4(pos + vec3(-size, size,0), 1);
	gl_Position = trans + vec4(-size, size,0,1);
	fragColor = vertColor[0];
	EmitVertex();

	// Vert 4
//	gl_TexCoord[0].st = vec2(-1,-1);
//	gl_Position = MVP * vec4(pos) + vec3(-size, -size,0), 1);
	gl_Position = trans + vec4(-size, -size,0,1);
	fragColor = vertColor[0];
	EmitVertex();

	EndPrimitive();
}