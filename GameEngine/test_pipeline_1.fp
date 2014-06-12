#version 330

in vec4 interpolation_position;
in vec4 interpolation_color;
in vec4 interpolation_normal;
in vec2 interpolation_texCoord;

layout (location = 0) out fragcolor;

void main()
{
	fragcolor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}