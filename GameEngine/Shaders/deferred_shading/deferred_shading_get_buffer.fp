#version 330

in VertexData {
	vec3 inter_normal;
	vec3 inter_tangent;
	vec2 inter_texCoord;
} VertexIn;

uniform vec4 diffuse_color;
uniform float diffuse_ratio;
uniform sampler2D diffuse_map;
uniform vec3 specular_color;
uniform float specular_ratio;
uniform float shininess_ratio;
uniform sampler2D normal_map;

layout (location = 0) out vec4 diffuse_frag;
layout (location = 1) out vec4 normal_frag;
layout (location = 2) out vec4 specular_frag;

vec3 perturb_normal()
{
	vec3 perturbated_normal_ts = texture(normal_map, VertexIn.inter_texCoord).xyz;
	vec3 perturbated_normal = perturbated_normal_ts * 2.f - vec3(1.f);
	vec3 bitangent = cross(VertexIn.inter_tangent, VertexIn.inter_normal);
	mat3 TBN = mat3(VertexIn.inter_tangent, bitangent, VertexIn.inter_normal);
	return mix(normalize(TBN * perturbated_normal), VertexIn.inter_normal, float(all(equal(perturbated_normal_ts, vec3(0.f)))));
}

void main(void)
{
	diffuse_frag = texture(diffuse_map, VertexIn.inter_texCoord); // diffuse_color * diffuse_ratio
	vec3 normal = perturb_normal() * 0.5f + 0.5f;
	normal_frag = vec4(normal, 1.0f);
	specular_frag = vec4(vec3(specular_color), shininess_ratio);
}