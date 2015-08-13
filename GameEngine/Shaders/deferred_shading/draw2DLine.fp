#version 330

layout (location = 0) out vec4 pxlColor;

in vec3 fColor;

void main(void)
{
	pxlColor = vec4(fColor, 1);
}