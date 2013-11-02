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

uniform layout (location = 1) sampler2D prePassTexture;

in vec4 gColor;
in vec2 gUv;

out layout (location = 0) vec4 FragColor;

void main(void)
{
    FragColor =	gColor * texture(prePassTexture, gUv);
}
