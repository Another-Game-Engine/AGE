#version 430

uniform mat4	projection;
uniform mat4	view;
uniform uvec2 	ctxSize;
uniform int 	sampleNbr;

in vec2 vUv;

uniform layout (location = 0) sampler2DMS layer0;

out vec4 frag_color;

void main ()
{
	ivec2 		coords = ivec2(vUv * vec2(ctxSize));
	vec4 		accum = texelFetch(layer0, coords, 0);

	for (int i = 1; i < sampleNbr; ++i)
        accum += texelFetch(layer0, coords, i);
    accum /= sampleNbr;
	frag_color = accum;
}