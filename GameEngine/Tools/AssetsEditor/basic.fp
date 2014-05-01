#version 400

uniform	mat4 projection;
uniform	mat4 view;
uniform	mat4 model;
uniform vec4 color;

out vec4 FragColor;
in vec4 boneColor;

void main(void)
{
	FragColor = vec4(1,1,1,1) * boneColor;
}
