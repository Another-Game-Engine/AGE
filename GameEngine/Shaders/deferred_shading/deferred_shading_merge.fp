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
	light = light * vec3(0.5f) / avg_log_luminance;
	light = light / (vec3(1.0f) + light);
	specular = specular * vec3(0.5f) / avg_log_luminance;
	specular = specular / (vec3(1.0f) + specular);
	diffuse = diffuse * (light + ambient_color);
	color = vec4(diffuse + specular, 1);
}