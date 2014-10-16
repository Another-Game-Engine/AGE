#version 330

in vec3 inter_normal;
in vec2 inter_texCoord;
in vec4 inter_position;
flat in vec3 eyes_pos;

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

mat3 cotangent_frame()
{
	vec3 eyesToPosition = normalize(eyes_pos - vec3(vec3(inter_position) / inter_position.w));
	vec3 dp1 = dFdx(eyesToPosition);
	vec3 dp2 = dFdy(eyesToPosition);
	vec2 duv1 = dFdx(inter_texCoord);
	vec2 duv2 = dFdy(inter_texCoord);
	
	vec3 dp2perp = cross(dp2, inter_normal);
	vec3 dp1perp = cross(inter_normal, dp1);
	vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
	vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;
	float invmax = inversesqrt(max(dot(T, T), dot(B, B)));
	return mat3(T * invmax, B * invmax, inter_normal);
}

vec3 perturb_normal()
{
	vec3 normal_texture_ts = texture(bump_texture, inter_texCoord).xyz;
	vec3 normal_texture = normal_texture_ts * 2.f - vec3(1.f);
	mat3 TBN = cotangent_frame();
	return mix(normalize(TBN * normal_texture), inter_normal, all(equal(normal_texture_ts, vec3(0.f))));
}

void main(void)
{
	diffuse_frag = (diffuse_color * diffuse_ratio) * texture(diffuse_texture, inter_texCoord);
	vec3 normal = perturb_normal() * 0.5f + 0.5f;
	normal_frag = vec4(normal, 1.0f);
	specular_frag = vec4(vec3(specular_color) * specular_ratio, shininess);
}