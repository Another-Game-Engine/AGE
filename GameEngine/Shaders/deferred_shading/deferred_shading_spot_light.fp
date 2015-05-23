#version 330

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 shiny;

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform sampler2D normal_buffer;
uniform sampler2D depth_buffer;
uniform sampler2D specular_buffer;
uniform sampler2DShadow shadow_map;
uniform vec3 eye_pos;

uniform vec3 position_light;
uniform vec3 attenuation_light;
uniform vec3 direction_light;
uniform vec3 color_light;
uniform mat4 light_matrix;

uniform float spot_cut_off;
uniform float exponent_light;

in vec2 interpolated_texCoord;

vec3 getWorldPosition(float depth, vec2 screenPos, mat4 viewProj)
{
	vec4 worldPos = vec4(screenPos * 2.0f - 1.0f, depth * 2.0f - 1.0f, 1.0f);
	worldPos = inverse(viewProj) * worldPos;
	worldPos /= worldPos.w;
	return (vec3(worldPos));
}

const int samplingNbr = 4;

void main()
{
	mat4 viewProj = projection_matrix * view_matrix;
	float depth = texture(depth_buffer, interpolated_texCoord).z;
	vec3 worldPos = getWorldPosition(depth, interpolated_texCoord, viewProj);
	vec3 lightDir = vec3(position_light - worldPos);
	float dist = length(lightDir);
	vec3 normal = normalize(vec3(texture(normal_buffer, interpolated_texCoord).xyz) * 2.0f - 1.0f);
	float attenuation = max(attenuation_light.x + attenuation_light.y * dist + attenuation_light.z * dist * dist, 1.0f); 
	lightDir = normalize(lightDir);
	float cosTheta = dot(normal, lightDir);
	float lambert = max(0.0f, cosTheta);
	float effect = max(0.0f, dot(-lightDir, direction_light));
	effect = effect * step(spot_cut_off, effect);
	effect = pow(effect, exponent_light);
	vec3 worldPosToEyes = normalize(eye_pos - worldPos);
	vec3 reflection = reflect(normalize(-lightDir), normal);
	vec4 shininessColor = texture2D(specular_buffer, interpolated_texCoord);
	float specularRatio = clamp(pow(max(dot(reflection, worldPosToEyes), 0.0f), shininessColor.a), 0.0f, 1.0f);
	vec4 shadowPos = light_matrix * vec4(worldPos, 1.0f);
	shadowPos = vec4(vec3(shadowPos.xyz / shadowPos.w) * 0.5f + 0.5f, 1.0f);
	float bias = clamp(0.005f * tan(acos(cosTheta)), 0.f, 0.000001f);
	vec2 poissonDisk[samplingNbr] = vec2[](vec2(-0.94201624, -0.39906216), vec2(0.94558609, -0.74890725), vec2(-0.094184101, -0.92938870) , vec2(0.34495938, 0.29387760));
	float visibility = 0.0f;
	for (int index = 0; index < samplingNbr; ++index) {
		vec4 samplingPos = vec4(shadowPos.xy + poissonDisk[index] / 700.0f, shadowPos.zw);
		visibility += (1.0f / float(samplingNbr)) * textureProj(shadow_map, samplingPos, bias);
	}
	color = (vec4(vec3(lambert * color_light), 0.f) * effect / attenuation) * visibility;
	shiny= (vec4(shininessColor.xyz * specularRatio, 0.f) * effect / attenuation) * visibility;
}