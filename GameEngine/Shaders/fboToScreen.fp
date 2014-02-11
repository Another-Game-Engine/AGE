#version 430

in vec2 st;

uniform layout (location = 0) sampler2D layer0;

out vec4 frag_colour;

void main ()
{
	frag_colour = texture(layer0, st);
}