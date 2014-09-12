#version 430

in vec4 interpolated_position;
in vec4 interpolated_color;
in vec3 interpolated_normal;
in vec2 interpolated_texCoord;
in vec4 pos_light_viewspace;

uniform vec4 diffuse_color;
uniform float diffuse_ratio;

layout (location = 0) out vec4 fragcolor;

void main(void)
{
	vec3 light = vec3(pos_light_viewspace) / pos_light_viewspace.w;
	vec3 position = vec3(interpolated_position) / interpolated_position.w;
	vec3 lightVec = normalize(light - position);
	vec4 diffuse =  diffuse_color * diffuse_ratio;
	fragcolor = diffuse;
}