#version 330

 out vec4 color;

uniform sampler2D diffuse_map;
uniform float avg_log_luminance;

in vec2 interpolated_texCoord;

void main()
{
	vec4 diffuse = texture2D(diffuse_map, interpolated_texCoord);
	float Linput = dot(vec4(0.30, 0.59, 0.11, 0.0), diffuse);
	float L = Linput * avg_log_luminance;
	float Loutput = L / (1 + L);
	color = diffuse * Loutput;
	//color = diffuse;
}