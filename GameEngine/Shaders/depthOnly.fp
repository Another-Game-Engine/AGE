#version 430

layout (std140 shared) uniform PerFrame
{
	mat4 projection;
	mat4 view;
	float time;
};

layout (std140 shared) uniform PerModel
{
	mat4 model;
};

out layout (location = 0) vec4 FragColor;

void main(void)
{
  FragColor = vec4(1, 1, 1, 1);
}