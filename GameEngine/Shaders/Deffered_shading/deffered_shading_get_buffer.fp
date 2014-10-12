#version 330

in vec3 inter_normal;
in vec2 inter_texCoord;
in vec3 inter_bitTangent;
in vec3 inter_tangent;

uniform vec4 diffuse_color;
uniform float diffuse_ratio;
uniform vec4 specular_color;
uniform float specular_ratio;
uniform float shininess;
uniform sampler2D diffuse_texture;
uniform sampler2D bump_texture;

layout (location = 0) out vec4 diffuse_frag;
layout (location = 1) out vec4 normal_frag;
layout (location = 2) out vec4 specular_frag;

vec3 perturb_normal()
{
	vec3 normal_bump = texture(bump_texture, inter_texCoord).xyz;
	mat3 TBN = mat3(inter_tangent, inter_bitTangent, inter_normal);
	return mix(normalize(transpose(TBN) * (normal_bump * 2.f - 1.f)), inter_normal, all(equal(normal_bump, vec3(0.f))));
}

void main(void)
{
	diffuse_frag = (diffuse_color * diffuse_ratio) * texture(diffuse_texture, inter_texCoord);
	vec3 normal = perturb_normal() * 0.5f + 0.5f;
	normal_frag = vec4(normal, 1.0f);
	specular_frag = vec4(vec3(specular_color) * specular_ratio, shininess);
}