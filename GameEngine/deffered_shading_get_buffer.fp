#version 440

in vec4 inter_position;
in vec4 inter_color;
in vec3 inter_normal;
in vec2 inter_texCoord;

uniform vec4 diffuse_color;
uniform float diffuse_ratio;

layout (location = 0) out vec4 diffuse_frag;
layout (location = 1) out float normal_frag;

void main(void)
{
	vec3 position = vec3(inter_position) / inter_position.w;
	diffuse_frag = diffuse_color * diffuse_ratio;
	normal_frag = inter_normal;
}