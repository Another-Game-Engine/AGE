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

in vec4 fColor;
in vec2 fUv;

out vec4 FragColor;

void main(void)
{
  FragColor =	fColor + texture2D(prePassTexture, vec2(gl_FragCoord.x / 1400.0f, gl_FragCoord.y / 1000.0f));
}
