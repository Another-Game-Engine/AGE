#version 330

// texture coordinates from vertex shaders
in vec2 st;

// framebuffer texture sampler
uniform layout (location = 0) sampler2D layer0;
uniform layout (location = 1) sampler2D layer1;
uniform layout (location = 2) sampler2D layer2;
uniform layout (location = 3) sampler2D layer3;


// output fragment colour RGBA
out layout (location = 0) vec4 frag_colour;

void main () {
	vec4 colour;
	if (st.x > 0.5)
	colour = 0.5 * texture(layer1, st);
	else
	colour = texture(layer1, st);
	frag_colour	= colour;
}