#version 330

// texture coordinates from vertex shaders
in vec2 st;

// texture sampler
uniform sampler2D tex;

// output fragment colour RGBA
out vec4 frag_colour;

void main () {
	vec4 colour;
	if (st.x > 0.5)
	colour = 0.5 * texture(tex, st);
	else
	colour = texture(tex, st);
	frag_colour	= colour;
}