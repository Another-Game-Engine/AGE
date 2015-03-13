#version 330

 out vec4 color;

uniform sampler2D light_buffer;
uniform sampler2D diffuse_map;

in vec2 interpolated_texCoord;

void main()
{
	vec3 diffuse = texture(diffuse_map, interpolated_texCoord).xyz;
	vec3 light = texture(light_buffer, interpolated_texCoord).xyz;
	color = vec4(/*light * */diffuse * vec3(1,1,1), 1.0f);
	//	color = vec4(0,0,1, 1.0f);
}