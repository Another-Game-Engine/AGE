#version 330

 out vec4 color;

uniform sampler2D light_buffer;
uniform float avg_log_luminance;

in vec2 interpolated_texCoord;

void main()
{
	vec3 diffuse = texture2D(diffuse_map, interpolated_texCoord).xyz;
	float Linput = dot(vec4(0.30, 0.59, 0.11, 0.0), color);
	color = color * avg_log_luminance;
	float Louput = L / (1 + L);
	color *= Linput * avg_log_luminance;
}