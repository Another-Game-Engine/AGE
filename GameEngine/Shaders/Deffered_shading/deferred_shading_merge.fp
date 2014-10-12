#version 330

layout (location = 0) out vec4 color;

uniform sampler2D light_buffer;
uniform sampler2D diffuse_buffer;

in vec2 interpolated_texCoord;

void main()
{
	vec3 diffuse = texture(diffuse_buffer, interpolated_texCoord).xyz;
	vec3 light = texture(light_buffer, interpolated_texCoord).xyz;
	color = vec4((light + 1.0f) * diffuse, 1.0f);
}