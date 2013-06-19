#version 330 core

// uniforms
uniform mat4 MVP;
uniform float viewportWidth;
uniform vec3 cameraPosition;
uniform vec3 systemPosition;

// inputs
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in float size;
// Will need later
//layout(location = 2) in vec3 tangent;
//layout(location = 3) in vec3 bitangent;

// outputs
//out vec2 UV;
//out vec4 vertColor;
//out vec3 vertPosition;
//out float vertSize;

void main()
{
	if(position == vec3(0,0,0))
	{
		return;
	}
	vec3 worldPos = systemPosition + position;
	// Sets the final position of the vertex.
	gl_Position = MVP * vec4(position, 1);
//	gl_Position = vec4(position, 1);
//	vertPosition = worldPos;
//	vertPosition = vec4(MVP * vec4(position, 1)).xyz;

//	vertSize = (size/2) * (viewportWidth / length(cameraPosition - worldPos));
	gl_PointSize = size * (viewportWidth / length(cameraPosition - worldPos));
	
//	gl_PointSize = 10;

//	vertColor = color;
	gl_FrontColor = color;
}