#version 430

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

out layout (location = 0) vec4 FragColor;

void main(void)
{
  FragColor = vec4(1, 1, 1, 1);
}