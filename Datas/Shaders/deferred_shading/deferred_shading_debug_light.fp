#version 330

uniform sampler2D sprite_light_map;

in vec2 interpolated_texCoord;
out vec4 color;

void main()
{
	vec4 billboardColor = texture(sprite_light_map, interpolated_texCoord);

	if (billboardColor.a < 0.5)
		discard;
	color = billboardColor;
}