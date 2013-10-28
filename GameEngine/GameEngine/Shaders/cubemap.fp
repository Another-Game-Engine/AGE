#version 330

layout (std140) uniform cameraUniform
{
	mat4 projection;
	mat4 view;
};

uniform samplerCube cube_texture;

in vec3 texcoords;

out vec4 frag_colour;

void main () {
  frag_colour = texture (cube_texture, texcoords);
}