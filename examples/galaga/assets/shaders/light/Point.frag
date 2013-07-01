#version 330 core

// general
uniform vec3 lightPosition;
uniform vec3 lightColor = vec3(1);
//uniform float ambientIntensity = 1.0f;
//uniform float diffuseIntensity = 1.0f;
//uniform float specularIntensity = 1.0f;
//uniform float specularPower;
//uniform vec3 attenuation = vec3(1);
uniform vec2 screenSize;
uniform vec3 cameraPosition;
uniform float radius;

uniform mat4 VP;
uniform mat4 M;

// textures
//uniform sampler2D depthTex;
uniform sampler2D positionTex;
uniform sampler2D normalTex;
uniform sampler2D diffuseTex;

// in
in mat4 inverseVP;
in vec2 vertUV;
in vec3 vertPos;

// out, write to light texture
layout(location=0) out vec4 diffuseColor;
//layout(location=4) out vec4 specularColor;

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / screenSize;
}

void main()
{
	/*
	vec3 pos = vec3(gl_FragCoord.xy / screenSize, 1);
	vec3 position = texture(positionTex, pos.xy).xyz;
//	vec3 depth = texture(depthTex, pos.xy).xyz;
	vec3 normal = normalize(texture(normalTex, pos.xy).rgb * 2 - 1);
	pos.z = 0.9f;

	vec4 clip = inverseVP * vec4(pos*2 - 1, 1);
	pos = clip.xyz / clip.w;

	float dist = length(lightPosition - pos);
	float atten = 1 - clamp(dist / radius, 0.0f, 1.0f);
//	float atten = 1 - dot(dist/radius, dist/radius);

	if(atten == 0.0f)
	{
		discard;
	}

	vec3 incident = normalize(lightPosition - pos);
	vec3 viewDir = normalize(position - pos);
	vec3 halfDir = normalize(incident + viewDir);

	float lambert = clamp(dot(incident, normal), 0, 1);
	float rFactor = clamp(dot(halfDir, normal), 0, 1);
	float sFactor = pow(rFactor, 33.0f);

	diffuseColor = vec4(lightColor.rgb * lambert * atten, 1);
//	specularColor = vec4(lightColor.rgb * sFactor * atten*0.33f, 1);
	*/
/*
	vec2 coord = gl_FragCoord.xy / screenSize; //sPos.xy/sPos.w*0.5+0.5;
	vec4 pos = texture2D(positionTex, coord); //get the position from deferred shading

	vec3 VP = lightPosition - pos.xyz; //vector between light and point
	float d = length(VP); //get the distance between the light and point
	if(d > radius) discard; //if outside of area of effect, discard pixel
	VP /= d; //normalize vector between light and point (divide by distance)
	vec4 norm = texture2D(normalTex, coord); //get the normal from deferred shading
	vec4 col = texture2D(diffuseTex, coord); //get the color from deferred shading
//	vec4 attr = texture2D(attrMap, coord); //get lighting attributes from deferred shading
	vec4 attr = vec4(1.0f);
	float diff_coeff = attr.r;
	float phong_coeff = attr.g;
	float two_sided = attr.b;
	float cost = dot(norm.xyz, VP);
	cost = (cost < 0.0)?-two_sided*cost:cost; //calculate two sided lighting.
	float diff = diff_coeff*cost; //calculate diffuse shading
	vec3 H = normalize(VP+normalize(cameraPosition - pos.xyz)); //calculate half vector
	float phong = phong_coeff*pow(max(dot(H, norm.xyz), 0.0), 100.0); //calculate phong shading
	vec3 C = lightColor*(vec4(1.0f)*diff+phong)/(d*d+0.8); //calculate light contribution with attenuation
	//all lights have constant quadratic attenuation of 1.0, with a constant attenuation of 0.8 to avoid dividing by small numbers

    diffuseColor = vec4(C, 1.0); //output color
    */
    /*
	vec2 uv = CalcTexCoord();
//	vec4 diffuse = texture2D(diffuseTex, uv);
	vec4 position = texture2D(positionTex, uv);
	vec4 normal = normalize(texture2D(normalTex, uv));
	vec3 dir = normalize(lightPosition - position.xyz);
	vec3 eyeDir = normalize(cameraPosition - position.xyz);
	vec3 halfVector = normalize(dir + eyeDir);

	diffuseColor = max(dot(normal, dir), 0) * diffuse + pow(max(dot(normal, halfVector), 0.0f), 100) * 1.5f;
	*/
}