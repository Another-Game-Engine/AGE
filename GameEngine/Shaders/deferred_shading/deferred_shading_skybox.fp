#version 330

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 lightAccumulation;

in vec3 texCoord;

uniform samplerCube skybox;

void main(void)
{
	vec3 vector = vec3(texCoord.x, texCoord.y, texCoord.z);
	color = vec4(texture(skybox, vector).xyz, 1.0f);
	lightAccumulation = vec4(1);
}