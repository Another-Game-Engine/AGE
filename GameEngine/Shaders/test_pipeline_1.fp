#version 430

in vec4 interpolated_color;

layout (location = 0) out vec4 fragcolor;

void main(void)
{
	fragcolor = vec4(interpolated_color.rgb, 1.f) + vec4(0.1f, 0.1f, 0.1f, 0.0f);
}