#version 430 core

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

layout(binding = 0) uniform sampler2D fTexture0;

in vec4 fColor;
in vec2 fUv;

out layout (location = 0) vec4 FragColor;

void main(void)
{
  FragColor = texture2D(fTexture0, fUv);
}
