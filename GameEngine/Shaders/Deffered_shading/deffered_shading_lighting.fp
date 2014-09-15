#include 330

layout (location = 0) out vec4 color;

uniform global_state
{
	mat4 projection_matrix;
};

uniform vec3 position_light;
uniform vec3 color_light;
uniform float power_light;
uniform float range_light;

uniform mat4 view_matrix;

uniform sampler2D depth_scene;
uniform sampler2D normal_scene;

in vec2 interpolated_texCoord;

vec3 getWoldPosition(float depth, vec2 screenPos, mat4 viewProj)
{
	vec4 worldPos = vec4(screenPos, depth, 1.0f);
	worldPos = inverse(viewProj) * worldPos;
	worldPos = worldPos / worldPos.w;
	return (vec3(worldPos));
}

void main()
{
	mat4 modelView = projection_matrix * view_matrix;
	float depth = texture(depth_scene, interpolated_texCoord).x;
	vec3 worldPos = getWorldPosition(depth, interpolated_texCoord, modelView);
	vec3 lightDir = worldPos - position_light;
	float lambert = max(0.0f, dot(normalize(texture(normal_scene, interpolated_texCoord).xyz * 2 - 1)), normalize(lightDir)));
	color = lambert;	
}