#version 330
#extension GL_ARB_texture_query_levels : enable

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
	vec4 light;
	float time;
};

layout (std140) uniform PerModel
{
	mat4 model;
};

layout (std140) uniform LightBias
{
  mat4 lightBiasMVP;
};

uniform          sampler2D fTexture0;//ambient;
uniform          sampler2DShadow fTexture1;//diffuse;
uniform          sampler2D fTexture2;//specular;
uniform          sampler2D fTexture3;//normal;
uniform			 sampler2D fTexture4;//shadowMap;

in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;
in vec4 ShadowCoord;

out layout (location = 0) vec4 FragColor;
out layout (location = 1) vec4 Lol;

float random_number(vec3 seed, int index)
{
	vec4 seed4 = vec4(seed, index);
	float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return fract(sin(dot_product) * 43758.5453);
}

void main(void)
{
    vec2 poissonDisk[4] = vec2[](vec2(-0.94201624, -0.39906216),
								 vec2(0.94558609, -0.76890725),
								 vec2(-0.094184101, -0.92938870),
								 vec2(0.34495938, 0.2987760));
	float visibility = 1.0;
	vec4 lightPos = (view * vec4(0.0, 20.0, -20.0, 1.0));
	vec3 vectorLight = normalize(vec3((lightPos.xyz / lightPos.w) - (fPosition.xyz / fPosition.w)));
	float bias = 0.005 * tan(acos(dot(fNormal.xyz, vectorLight)));
	float lamberTerm = clamp(dot(fNormal.xyz, vectorLight), 0.0, 1.0);
	for (int i = 0; i < 4; i++)
	{
		int index = int(4.0 * random_number(floor(fPosition.xyz * 2000.0), i)) % 4;
		visibility -= 0.2 * (1.0 - texture(fTexture1, vec3(ShadowCoord.xy + poissonDisk[index] / 2000.0, ShadowCoord.z - bias)));
	}
	FragColor = vec4(vec3(1.0, 1.0, 1.0) * lamberTerm * visibility, 1.0);
}