#version 330

// Uniforms
uniform vec2 viewport;
uniform vec3 cameraPos;

// Inputs
layout(location = 0) in float size;
layout(location = 1) in float rotation;
layout(location = 2) in uvec4 color;

// Outputs

void main()
{
	gl_Position = ftransform();
	gl_PointSize = size * viewport.x / distance(cameraPos, gl_Position.xyz);
	gl_Color = color/255;
}