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

uniform layout (location = 0) sampler2D layer0;
uniform layout (location = 1) sampler2D layer1;
uniform layout (location = 2) sampler2D layer2;
uniform layout (location = 3) sampler2D layer3;


in vec4 gColor;
in vec2 gUv;

out layout (location = 0) vec4 FragColor;

void main(void)
{
    FragColor =	vec4(1,0,1,1);// * texture(layer1, gUv / vec2(1920, 1080));
}
