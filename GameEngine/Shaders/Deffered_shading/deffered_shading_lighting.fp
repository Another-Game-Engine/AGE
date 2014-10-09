#version 330

layout (location = 0) out vec4 color;

uniform global_state
{
	mat4 projection_matrix;
	mat4 view_matrix;
};

uniform vec3 position_light;
uniform vec3 attenuation_light;
uniform vec3 color_light;

uniform sampler2D depth_buffer;
uniform sampler2D normal_buffer;
uniform sampler2D specular_buffer;

uniform float shininess;

in vec2 interpolated_texCoord;
flat in vec3 interpolated_eyes_pos;

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
	vec3 lightDir = position_light - worldPos;
	float dist = length(lightDir);
	vec3 normal = normalize(vec3(texture(normal_buffer, interpolated_texCoord).xyz) * 2.0f - 1.0f);
	float attenuation = attenuation_light.x + attenuation_light.y * dist + attenuation_light.z * dist * dist; 
	float lambert = max(0.0f, dot(normal, normalize(lightDir)));
	vec3 worldPosToEyes = normalize(interpolated_eyes_pos - worldPos);
	vec3 reflection = reflect(normalize(-lightDir), normal);
	vec4 specular = texture(specular_buffer, interpolated_texCoord);
	float specularRatio = clamp(pow(max(dot(reflection, worldPosToEyes), 0.0f), 100.f * specular.w), 0.0f, 1.0f);
	color = vec4(vec3(lambert * color_light + vec3(specular) * specularRatio), 1.0f) / (attenuation) * (1.f - step(1.0f, depth));
}