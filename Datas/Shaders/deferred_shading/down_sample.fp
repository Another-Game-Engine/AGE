#version 330

uniform sampler2D sourceTexture;
uniform vec2 inverseSourceSize;

out vec4 color;

in vec2 interpolated_texCoord;

void main()
{
	vec3 finalColor = vec3(0);

	finalColor += texture(sourceTexture, interpolated_texCoord + vec2(0.5, 0.5) * inverseSourceSize).rgb;
	finalColor += texture(sourceTexture, interpolated_texCoord + vec2(0, 0.5) * inverseSourceSize).rgb;
	finalColor += texture(sourceTexture, interpolated_texCoord + vec2(0, 0) * inverseSourceSize).rgb;
	finalColor += texture(sourceTexture, interpolated_texCoord + vec2(0.5, 0) * inverseSourceSize).rgb;

	color = vec4(finalColor / 4.0f, 1);
}