#version 430

layout (std140) uniform PerFrame
{
	mat4 projection;
	mat4 view;
	int	lightNbr;
	float time;
};

in vec2 vUv;

uniform layout (location = 0) sampler2D layer0;

out vec4 frag_color;

void main ()
{
	frag_color = texture(layer0, vUv);
}