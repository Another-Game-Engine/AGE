#version 430 core

layout (std140) uniform MaterialBasic
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 transmittance;
	vec3 emission;
	float shininess;
};

layout (std140) uniform PerFrame
{
	mat4 projection;
	mat4 view;
	int	lightNbr;
	float time;
};

layout (std140) uniform PerModel
{
	mat4 model;
};

layout(binding = 0) uniform sampler2D fTexture0; //ambient;
layout(binding = 1) uniform sampler2D fTexture1; //diffuse;
layout(binding = 2) uniform sampler2D fTexture2; //specular;
layout(binding = 3) uniform sampler2D fTexture3; //normal;

struct PointLight
{
	vec4	position;
	vec4	colorRange;
};

layout(std430, binding = 4) buffer lightBuff
{
	PointLight	lights[];
};

in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out layout (location = 0) vec4 FragColor;

void main(void)
{
	vec4	finalColor = vec4(0, 0, 0, 1);

	for (int i = 0; i < lightNbr; ++i)
	{
		vec4	lightPos = view * lights[i].position;
		vec3	lightColor = lights[i].colorRange.xyz;
		float	lightRange = lights[i].colorRange.w;

		vec3	fragToLight = lightPos.xyz - fPosition.xyz;
		float	dist = length(fragToLight);
		float	illumination =  clamp(dot(normalize(fragToLight), normalize(fNormal.xyz)), 0.0, 1.0) *
								clamp(1.0f - dist / lightRange, 0.0f, 1.0f);

		finalColor += vec4(vec3(illumination) *
						   lightColor *
						   texture2D(fTexture1, fTexCoord).xyz, 0.0f);
	}
	FragColor = finalColor;
}
