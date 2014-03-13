#version 330

layout (std140) uniform PerModel
{
	mat4 model;
};

layout (std140) uniform PerLight
{
	mat4 lightVP;
};

layout (location = 0) out float FragColor;

void main()
{
	FragColor = vec4(0);
}