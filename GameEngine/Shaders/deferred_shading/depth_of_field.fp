#version 330

#define NEAR_BLUR_DISTANCE 2.0
#define NEAR_FOCUS 4.0
#define FAR_BLUR_DISTANCE 10.0
#define FAR_FOCUS 7.0
#define NEAR 0.1
#define FAR 2000

uniform sampler2D cleanMap;
uniform sampler2D blurredMap;
uniform sampler2D depthMap;

out vec4 color;

in vec2 interpolated_texCoord;

float linearizeDepth(float depth, float near, float far)
{
    float clipDepth = depth * 2.0 - 1.0;
    return (2.0 * near * far / (far + near - clipDepth * (far - near)));
}

void main()
{
	vec3 blurredColor = texture(blurredMap, interpolated_texCoord).rgb;
	vec3 cleanColor = texture(cleanMap, interpolated_texCoord).rgb;
	float linearized = linearizeDepth(texture(depthMap, interpolated_texCoord).r, NEAR, FAR);

	float ratio = 1;

	if (linearized < NEAR_FOCUS)
		ratio = 1.0 - (NEAR_FOCUS - linearized) / (NEAR_BLUR_DISTANCE);
	else if (linearized > FAR_FOCUS)
		ratio = 1.0 - (linearized - FAR_FOCUS) / (FAR_BLUR_DISTANCE);
	ratio = clamp(ratio, 0, 1);
	color = vec4(mix(blurredColor, cleanColor, ratio), 1);
}