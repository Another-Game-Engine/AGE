#version 330

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

uniform	sampler2D prePassTexture;

in vec4 gColor;
in vec2 gUv;

out vec4 FragColor;

void main(void)
{
    FragColor =	gColor * texture(prePassTexture, gUv);
}
