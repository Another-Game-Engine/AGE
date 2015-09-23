#version 330

uniform sampler2D cleanMap;
uniform sampler2D blurredMap1;
uniform sampler2D blurredMap2;

in vec2 fUv;

out vec4 color;

void main()
{
	vec3 cleanMapColor = texture(cleanMap, fUv).rgb;
	vec3 blurredMap1Color = max(texture(blurredMap1, fUv).rgb - vec3(1.0f), 0);
	vec3 blurredMap2Color = max(texture(blurredMap2, fUv).rgb - vec3(1.0f), 0);

	color = vec4(cleanMapColor + (blurredMap1Color + blurredMap2Color) * 0.5f, 1);
}