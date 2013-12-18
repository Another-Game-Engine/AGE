#version 330
#extension GL_ARB_separate_shader_objects: enable
// framebuffer texture sampler
uniform layout (location = 0) sampler2D layer0;
uniform layout (location = 1) sampler2D layer1;
uniform layout (location = 2) sampler2D layer2;
uniform layout (location = 3) sampler2D layer3;

layout (std140) uniform cameraUniform
{
	mat4 projection;
	mat4 view;
};

uniform samplerCube cube_texture;

in vec3 texcoords;

out layout (location = 0) vec4 frag_colour;

void main () {
  frag_colour = texture (cube_texture, texcoords);
}