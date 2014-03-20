#version 330

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
uniform          sampler2D fTexture1;//diffuse;
uniform          sampler2D fTexture2;//specular;
uniform          sampler2D fTexture3;//normal;
uniform			    sampler2D fTexture4;//shadowMap;

in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;
in vec4 ShadowCoord;

out layout (location = 0) vec4 FragColor;
out layout (location = 1) vec4 Lol;

void main(void)
{
	float visibility = 1.0;
	vec4 lightPos = (view * vec4(0.0, 20.0, -20.0, 1.0));
	vec3 vectorLight = normalize(vec3((lightPos.xyz / lightPos.w) - (fPosition.xyz / fPosition.w)));
	float bias = 0.005 * tan(acos(dot(fNormal.xyz, vectorLight)));
	float lamberTerm = clamp(dot(fNormal.xyz, vectorLight), 0.0, 1.0);
		if (texture(fTexture1, ShadowCoord.xy).z < (ShadowCoord.z - bias))
			visibility *= 0.5;
	FragColor = vec4(vec3(1.0, 1.0, 1.0) * lamberTerm * visibility, 1.0) ;
}
