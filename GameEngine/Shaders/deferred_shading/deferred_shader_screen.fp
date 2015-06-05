#version 330

uniform sampler2D screen;

layout (location = 0) out vec4 color;

in vec2 texCoord;

void main(void)
{
	color = texture2D(screen, texCoord);
}