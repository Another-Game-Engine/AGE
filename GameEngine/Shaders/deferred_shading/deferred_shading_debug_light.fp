#version 330

uniform sampler2D sprite_light_map;

in vec2 interpolated_texCoord;
out vec4 color;

void main()
{
	color = texture(sprite_light_map, interpolated_texCoord);
}