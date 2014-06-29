#version 440

in vec4 interpolated_position;
in vec4 interpolated_color;
in vec4 interpolated_normal;
in vec2 interpolated_texCoord;

uniform sampler2D diffuse_texture;
uniform vec4 diffuse_color;
uniform float diffuse_ratio;

layout (location = 0) out vec4 fragcolor;

void main(void)
{
	vec4 diffuse = interpolated_color * texture2D(diffuse_texture, interpolated_texCoord) * diffuse_color * diffuse_ratio;
	fragcolor = diffuse;
}