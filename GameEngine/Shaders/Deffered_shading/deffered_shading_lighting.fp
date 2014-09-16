#version 330

layout (location = 0) out vec4 color;

uniform global_state
{
	mat4 projection_matrix;
	mat4 view_matrix;
};

uniform vec3 position_light;
uniform float range_light;

uniform sampler2D depth_buffer;
uniform sampler2D normal_buffer;

in vec2 interpolated_texCoord;

vec3 getWorldPosition(float depth, vec2 screenPos, mat4 viewProj)
{
	vec4 worldPos = vec4(screenPos, depth, 1.0f);
	worldPos = inverse(viewProj) * worldPos;
	worldPos = worldPos / worldPos.w;
	return (vec3(worldPos));
}

void main()
{
	mat4 modelView = projection_matrix * view_matrix;
	float depth = texture(depth_buffer, interpolated_texCoord).x;
	vec3 worldPos = getWorldPosition(depth, interpolated_texCoord, modelView);
	float dist = distance(worldPos, position_light);
	float attenuation = 1.0f;
	float lambert = 0.f;
	if (dist < range_light)
	{
		attenuation = 1.0f / (1.0f + 0.1f * dist + 0.01f * dist * dist);
		vec3 lightDir = worldPos - position_light;
		lambert = max(0.0f, dot(normalize((texture(normal_buffer, interpolated_texCoord).xyz * 2.0f - 1.0f)), normalize(lightDir)));
	}
	color = vec4(lambert * attenuation);
}