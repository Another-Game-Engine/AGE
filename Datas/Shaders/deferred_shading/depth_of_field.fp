#version 330

#define NEAR_BLUR_DISTANCE 2.0
#define FAR_BLUR_DISTANCE 20.0
#define NEAR 0.1
#define FAR 2000

uniform sampler2D cleanMap;
uniform sampler2D blurredMap1;
uniform sampler2D blurredMap2;
uniform sampler2D depthMap;

out vec4 color;

in vec2 interpolated_texCoord;
flat in float centerDepth;

float linearizeDepth(float depth, float near, float far)
{
    float clipDepth = depth * 2.0 - 1.0;
    return (2.0 * near * far / (far + near - clipDepth * (far - near)));
}

void main()
{
	vec3 blurredColor1 = texture(blurredMap1, interpolated_texCoord).rgb;
	vec3 blurredColor2 = texture(blurredMap2, interpolated_texCoord).rgb;
	vec3 cleanColor = texture(cleanMap, interpolated_texCoord).rgb;
	float linearized = linearizeDepth(texture(depthMap, interpolated_texCoord).r, NEAR, FAR);

	float nearFocus = clamp(centerDepth / 4.0f, NEAR + 0.2, NEAR + 10.0);
	float farFocus = min(centerDepth + centerDepth / 7.0f, FAR);

	float nearBlurDistance = (nearFocus) / 2.0f;
	float farBlurDistance = 20.0;

	float ratio = 1;

	if (linearized < nearFocus)
		ratio = 1.0 - (nearFocus - linearized) / (nearBlurDistance);
	else if (linearized > farFocus)
		ratio = 1.0 - (linearized - farFocus) / (farBlurDistance);
	ratio = clamp(ratio, 0, 1);

	if (ratio < 0.5f)
		color = vec4(mix(blurredColor2, blurredColor1, ratio * 2.0f), 1);
	else
		color = vec4(mix(blurredColor1, cleanColor, (ratio - 0.5f) * 2.0f), 1);
}