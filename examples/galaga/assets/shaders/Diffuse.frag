#version 330 core

// uniforms
uniform sampler2D textureSampler;

// inputs
in vec2 UV;

// outputs
out vec3 color;

void main()
{
    // Modify color here.
    color = texture2D( textureSampler, UV ).rgb;
    //color = vec3(0.6, 0.2, 0);
}
