#version 330

// texture coordinates from vertex shaders
in vec2 st;

// texture sampler
layout (location = 0) uniform sampler2D tex;

// output fragment colour RGBA
out vec4 frag_colour;

void main () {
	vec4 colour = texture(tex, st);
	frag_colour	= colour;
}