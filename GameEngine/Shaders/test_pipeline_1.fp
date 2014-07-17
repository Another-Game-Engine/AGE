#version 440

in vec4 interpolated_position;
in vec4 interpolated_color;
in vec3 interpolated_normal;
in vec2 interpolated_texCoord;
in vec4 pos_light_viewspace;

uniform sampler2D diffuse_texture;
uniform vec4 diffuse_color;
uniform float diffuse_ratio;

layout (location = 0) out vec4 fragcolor;

void main(void)
{
	vec3 light = vec3(pos_light_viewspace) / pos_light_viewspace.w;
	vec3 position = vec3(interpolated_position) / interpolated_position.w;
	vec3 lightVec = normalize(light - position);
	float lambert_intensity = clamp(dot(lightVec, interpolated_normal), 0.0f, 1.0f);
	vec4 diffuse = interpolated_color * texture2D(diffuse_texture, interpolated_texCoord) * diffuse_color * diffuse_ratio * lambert_intensity;
	fragcolor = diffuse;
}