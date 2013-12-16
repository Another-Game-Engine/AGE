#version 330

// texture coordinates from vertex shaders
in vec2 st;

// framebuffer texture sampler
uniform layout (location = 0) sampler2D layer0;

// output fragment colour RGBA
out vec4 frag_colour;

void main () {
	frag_colour = texture(layer0, st);
}