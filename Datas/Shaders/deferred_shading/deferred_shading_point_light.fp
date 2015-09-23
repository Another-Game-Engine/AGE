#version 330

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 shiny;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;

uniform vec3 position_light;
uniform vec3 attenuation_light;
uniform vec3 color_light;
uniform vec3 ambient_color;

uniform vec3 eye_pos;

uniform sampler2D depth_buffer;
uniform sampler2D normal_buffer;
uniform sampler2D specular_buffer;

noperspective in vec2 interpolated_texCoord;

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
	float depth = texture(depth_buffer, interpolated_texCoord).x;
	vec3 worldPos = getWorldPosition(depth, interpolated_texCoord, viewProj);
	vec3 lightDir = vec3(position_light) - worldPos;
	float dist = length(lightDir);
	vec3 normal = normalize(vec3(texture(normal_buffer, interpolated_texCoord).xyz) * 2.0f - 1.0f);
	float attenuation = attenuation_light.x + attenuation_light.y * dist + attenuation_light.z * dist * dist; 
	float lambert = max(0.0f, dot(normal, normalize(lightDir)));
	vec3 worldPosToEyes = normalize(eye_pos - worldPos);
	vec3 reflection = reflect(normalize(-lightDir), normal);
	vec4 shininessColor = texture2D(specular_buffer, interpolated_texCoord);
	float specularRatio = clamp(pow(max(dot(reflection, worldPosToEyes), 0.f), 150.f * shininessColor.a), 0.0f, 1.0f);
	color = vec4(lambert * color_light, 0.f) / attenuation;
	shiny = vec4(shininessColor.xyz * specularRatio, 0.f) / attenuation;
}