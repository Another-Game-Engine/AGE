#version 330

 out vec4 color;

uniform sampler2D light_buffer;
uniform sampler2D shiny_buffer;
uniform sampler2D diffuse_map;

uniform vec3 ambient_color;
uniform float avg_log_luminance;

in vec2 interpolated_texCoord;

void main()
{
	vec3 diffuse = texture2D(diffuse_map, interpolated_texCoord).xyz;
	vec3 light = texture2D(light_buffer, interpolated_texCoord).xyz;
	vec3 specular = texture2D(shiny_buffer, interpolated_texCoord).xyz;
	diffuse = diffuse * (light + ambient_color);
	color = vec4(diffuse + specular, 1);
	//float Linput = dot(vec4(0.30, 0.59, 0.11, 0.0), color);
	//color = color * avg_log_luminance;
	//float Louput = L / (1 + L);
	//color *= Linput * avg_log_luminance;
}