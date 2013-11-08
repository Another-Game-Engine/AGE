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

uniform	sampler2D fTexture0;

in vec4 fColor;
in vec2 fUv;

out layout (location = 0) vec4 FragColor;

void main(void)
{
  FragColor = fColor * texture2D(fTexture0, fUv);
}
