#version 430

uniform mat4	projection;
uniform mat4	view;

in vec2 vUv;

uniform layout (location = 0) sampler2D layer0;

out vec4 frag_color;

void main ()
{
	frag_color = texture(layer0, vUv);
}