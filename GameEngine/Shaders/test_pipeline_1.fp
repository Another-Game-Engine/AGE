#version 430

uniform sampler2D diffuse_map;

in vec2 vTexCoord;

layout (location = 0) out vec4 fragcolor;

void main(void)
{
	vec4 texColor = texture(diffuse_map, vTexCoord);
	fragcolor = texColor;
}