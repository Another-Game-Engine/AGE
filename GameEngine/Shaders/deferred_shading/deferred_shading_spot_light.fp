#version 330

out vec4 color;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform sampler2D normal_buffer;
uniform sampler2D depth_buffer;
uniform vec3 eye_pos;

uniform vec3 position_light;
uniform vec3 attenuation_light;
uniform vec3 direction_light;
uniform vec3 color_light;
uniform vec3 ambiant_color;

in vec2 interpolated_texCoord;

vec3 getWorldPosition(float depth, vec2 screenPos, mat4 viewProj)
{
	vec4 worldPos = vec4(screenPos * 2.0f - 1.0f, depth * 2.0f - 1.0f, 1.0f);
	worldPos = inverse(viewProj) * worldPos;
	worldPos /= worldPos.w;
	return (vec3(worldPos));
}

void main()
{
	//some stuff here :p	
	color = vec4(1.0f);
}