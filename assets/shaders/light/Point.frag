#version 330 core

// general
uniform vec3 lightPosition;
uniform vec3 lightColor = vec3(1);
uniform float ambientIntensity = 1.0f;
uniform float diffuseIntensity = 1.0f;
uniform float specularIntensity = 1.0f;
uniform float specularPower;
uniform vec2 screenSize;
uniform vec3 cameraPosition;
uniform float radius;

uniform vec3 attenuation = vec3(1);

//uniform mat4 VP;
//uniform mat4 M;

// textures
uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D diffuseTex;
//uniform sampler2D depthTex;

// in
//in mat4 inverseVP;
//in vec2 vertUV;
//in vec3 vertPos;

// out, write to light texture
layout(location=0) out vec4 diffuseColor;
//layout(location=4) out vec4 specularColor;

vec4 CalcLightInternal(//BaseLight Light,
					   vec3 LightDirection,
					   vec3 WorldPos,
					   vec3 Normal)
{
    vec4 AmbientColor = vec4(lightColor, 1.0) * ambientIntensity;
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0.0) {
        DiffuseColor = vec4(lightColor, 1.0) * diffuseIntensity * DiffuseFactor;

        vec3 VertexToEye = normalize(cameraPosition - WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
        SpecularFactor = pow(SpecularFactor, specularPower);
        if (SpecularFactor > 0.0) {
            SpecularColor = vec4(lightColor, 1.0) * specularIntensity * SpecularFactor;
        }
    }

    return (AmbientColor + DiffuseColor + SpecularColor);
}

vec4 CalcPointLight(vec3 WorldPos, vec3 Normal)
{
    vec3 LightDirection = WorldPos - lightPosition;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(/*gPointLight.Base,*/ LightDirection, WorldPos, Normal);

    float Attenuation =  attenuation.x +
                         attenuation.y * Distance +
                         attenuation.z * Distance * Distance;

    Attenuation = max(1.0, Attenuation);

    return Color / Attenuation;
}

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / screenSize;
}

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 pos = texture2D(positionTex, texCoord).rgb;
	vec3 color = texture2D(diffuseTex, texCoord).rgb;
	vec3 normal = texture2D(normalTex, texCoord).rgb;
	normal = normalize(normal);

	diffuseColor = vec4(color,1.0) * CalcPointLight(pos, normal);
	diffuseColor.a = 1.0;
//	diffuseColor = vec4(pos, 1.0);
}