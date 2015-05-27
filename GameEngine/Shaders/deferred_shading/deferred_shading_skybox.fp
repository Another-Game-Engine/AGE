#version 330

layout (location = 0) out vec4 color;

in vec3 texCoord;

uniform samplerCube skybox;

void main(void)
{
	color = vec4(texture(skybox, texCoord).xyz, 1.0f);
}