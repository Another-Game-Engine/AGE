#version 330

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 shiny;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform sampler2D normal_buffer;
uniform sampler2D depth_buffer;
uniform sampler2D specular_buffer;
uniform vec3 eye_pos;

uniform vec3 direction_light;
uniform vec3 color_light;

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
	mat4 viewProj = projection_matrix * view_matrix;
	float depth = texture2D(depth_buffer, interpolated_texCoord).x;
	vec3 worldPos = getWorldPosition(depth, interpolated_texCoord, viewProj);
	vec3 normal = normalize(vec3(texture(normal_buffer, interpolated_texCoord).xyz) * 2.0f - 1.0f);
	float lambert = max(0.0f, dot(normal, direction_light));
	vec3 worldPosToEyes = normalize(eye_pos - worldPos);
	vec3 reflection = reflect(normalize(-direction_light), normal);
	vec4 shininessColor = texture2D(specular_buffer, interpolated_texCoord);
	float specularRatio = clamp(pow(max(dot(reflection, worldPosToEyes), 0.0f), 150.f * shininessColor.a), 0.0f, 1.0f);
	color = vec4(vec3(lambert * color_light), 0.f);
	shiny = vec4(vec3(specularRatio * shininessColor.xyz), 0.f);
}