#version 330

layout (std140) uniform PerFrame
{
	mat4 vProjection;
	mat4 vView;
};

layout (std140) uniform PerModel
{
	mat4 vModel;
};

in vec4 fColor;

out vec4 FragColor;

void main(void)
{
  FragColor = fColor;
}
