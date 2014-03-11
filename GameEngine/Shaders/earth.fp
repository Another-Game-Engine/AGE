#version 430 core

layout (std140) uniform PerFrame
{
        mat4 projection;
        mat4 view;
        float time;
};

layout (std140) uniform PerModel
{
        mat4 model;
};

layout(binding = 0) uniform sampler2D fTexture0; // ambient;
layout(binding = 1) uniform sampler2D fTexture1; // diffuse;
layout(binding = 2) uniform sampler2D fTexture2; // specular;
layout(binding = 3) uniform sampler2D fTexture3; // normal;

struct PointLight
{
	vec4	position;
	vec4	colorRange;
};

layout(binding = 4, std430) buffer lightBuff
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

//	for (uint i = 0; i < lightNbr; ++i)
//	{
//	  vec4 normal = vec4(normalize(fNormal.xyz + texture2D(fTexture3, fTexCoord).xyz), 0);
//
//	  vec3 lightPos = vec3(view * lights[i].position);
//	  vec4 vectorLight = normalize(vec4(lightPos - fPosition.xyz, 1.0));
//	  vec4 vectorReflect = normalize(reflect(-vectorLight, normal));
//	  vec4 vectorView = normalize(vec4(fPosition.xyz - view[3].xyz, 1.0));
//
//	  float lamberTerm = clamp(dot(normal.xyz, vectorLight.xyz), 0.03, 1.0);
//	  finalColor.xyz += lamberTerm * texture2D(fTexture0, fTexCoord).rgb +
//					(1.0 - lamberTerm) * texture2D(fTexture1, fTexCoord).rgb +
//					lamberTerm * texture2D(fTexture2, fTexCoord - vec2(time / 80, 0)).rgb;
//	}
	FragColor = finalColor;
}
