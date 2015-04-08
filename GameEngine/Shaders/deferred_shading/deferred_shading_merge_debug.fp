#version 330

 out vec4 color;

uniform sampler2D debug_light_map;

in vec2 interpolated_texCoord;

void main()
{
	color = texture(debug_light_map, interpolated_texCoord);
}