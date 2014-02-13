#version 430 core

// framebuffer texture sampler
uniform layout (location = 1) sampler2D layer1;
uniform layout (location = 2) sampler2D layer2;
uniform layout (location = 3) sampler2D layer3;

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

layout(binding = 0) uniform	sampler2D fTexture0;
layout(binding = 1) uniform	sampler2D fTexture1;

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
	for (uint i = 0; i < lightNbr; ++i)
	{
		vec4 bNormal = vec4(normalize(fNormal.xyz + texture2D(fTexture1, fTexCoord).xyz), 0);

		vec3 lightPos = (view * lights[i].position).xyz;
		vec4 vectorLight = normalize(vec4(lightPos - fPosition.xyz, 1.0));
		vec4 vectorReflect = normalize(reflect(-vectorLight, bNormal));
		vec4 vectorView = normalize(vec4(fPosition.xyz - view[3].xyz, 1.0));

		float lamberTerm = clamp(dot(bNormal.xyz, vectorLight.xyz), 0.0, 1.0);
		vec4 pxlColor = fColor * texture2D(fTexture0, fTexCoord);
		vec4 ambiant = pxlColor * vec4(0.05, 0.05, 0.05, 1.0);
		vec4 diffuse = pxlColor * lamberTerm;
		FragColor = max(ambiant, diffuse);
	}
}
