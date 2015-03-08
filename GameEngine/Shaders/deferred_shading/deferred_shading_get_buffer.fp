#version 330

in vec3 inter_normal;
in vec2 interpolated_texCoord;
in vec3 inter_tangent;

uniform vec4 diffuse_color;
uniform float diffuse_ratio;
uniform sampler2D diffuse_texture;
uniform vec4 specular_color;
uniform float specular_ratio;
uniform sampler2D specular_texture;
uniform float shininess;

uniform sampler2D normal_texture;

layout (location = 0) out vec4 diffuse_frag;
layout (location = 1) out vec4 normal_frag;
layout (location = 2) out vec4 specular_frag;

vec3 perturb_normal()
{
	vec3 perturbated_normal_ts = texture(normal_texture, interpolated_texCoord).xyz;
	vec3 perturbated_normal = perturbated_normal_ts * 2.f - vec3(1.f);
	vec3 bitangent = cross(inter_tangent, inter_normal);
	mat3 TBN = mat3(inter_tangent, bitangent, inter_normal);
	return mix(normalize(TBN * perturbated_normal), inter_normal, float(all(equal(perturbated_normal_ts, vec3(0.f)))));
}

void main(void)
{
	diffuse_frag = vec4(1,0,1,1)/*(diffuse_color * diffuse_ratio)*/ /** texture(diffuse_texture, interpolated_texCoord)*/;
	vec3 normal = perturb_normal() * 0.5f + 0.5f;
	normal_frag = vec4(normal, 1.0f);
	specular_frag = vec4(vec3(specular_color) * specular_ratio, shininess);
}