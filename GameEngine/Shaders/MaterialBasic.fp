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
	float time;
	int	lightNbr;
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
	vec3	position;
	vec3	color;
	float	range;
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
		vec3	fragToLight = lights[i].position - fPosition.xyz;
		float	dist = length(fragToLight);
		float	illumination =  clamp(dot(fNormal.xyz, fragToLight.xyz), 0.0, 1.0) * clamp(1.0f - dist / lights[i].range, 0, 1);

		finalColor += vec4(illumination, illumination, illumination, 1.0f) *
					  vec4(lights[i].color, 1.0f) *
					  texture2D(fTexture1, fTexCoord);
	}
	FragColor = finalColor;
}
