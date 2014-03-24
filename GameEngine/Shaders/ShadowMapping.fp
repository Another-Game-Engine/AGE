#version 330

in vec4 fColor;

layout (std140) uniform PerModel
{
	mat4 model;
};

layout (std140) uniform PerLight
{
	mat4 lightVP;
};

layout (location = 0) out vec4 FragColor;

void main()
{
	if (fColor.a == 0.0f)
		discard;
	FragColor = vec4(0);
}