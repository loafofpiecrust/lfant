#version 330 core

// uniforms
uniform mat4 model;
uniform mat4 projection;
uniform float viewportWidth;
uniform vec3 cameraPosition;
uniform vec3 systemPosition = vec3(0);

// inputs
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in float size;
// Will need later
//layout(location = 2) in vec3 tangent;
//layout(location = 3) in vec3 bitangent;

// outputs
//out vec2 UV;
out vec4 vertColor;
out float vertSize;

void main()
{
//	if(position == vec3(0,0,0) || color.a == 0.0f)
	{
	//	return;
	}
//	vec4 worldPos = vec4(systemPosition + position.xyz, 1);
	// Sets the final position of the vertex.
//	gl_Position = worldPos;
	gl_Position = model*vec4(position.xyz, 1);
//	vertPosition = worldPos;
//	vertPosition = vec4(MVP * vec4(position, 1)).xyz;

	vertSize = 0.5f;
//	vertSize = size / length(cameraPosition - worldPos.xyz) * 25;
//	vertSize = size * (viewportWidth / length(cameraPosition - worldPos.xyz));
//	gl_PointSize = size * (viewportWidth / length(cameraPosition - worldPos));
	
//	gl_PointSize = 10;

	vertColor = color;
//	gl_FrontColor = color;
}