#version 440

in vec4 interpolated_position;
in vec4 interpolated_color;
in vec4 interpolated_normal;
in vec2 interpolated_texCoord;

layout (location = 0) out vec4 fragcolor;

void main(void)
{
	fragcolor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}