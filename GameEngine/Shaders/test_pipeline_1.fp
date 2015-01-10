#version 430

in vec4 interpolated_color;

layout (location = 0) out vec4 fragcolor;

void main(void)
{
	fragcolor = interpolated_color;
}