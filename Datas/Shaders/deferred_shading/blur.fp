#version 330

uniform sampler2D sourceTexture;

out vec4 color;

in vec2 blurOffsets[5];

void main()
{
	vec3 finalColor = vec3(0);

	finalColor += texture(sourceTexture, blurOffsets[0]).rgb * 0.0702702703;
	finalColor += texture(sourceTexture, blurOffsets[1]).rgb * 0.3162162162;
	finalColor += texture(sourceTexture, blurOffsets[2]).rgb * 0.2270270270;
	finalColor += texture(sourceTexture, blurOffsets[3]).rgb * 0.3162162162;
	finalColor += texture(sourceTexture, blurOffsets[4]).rgb * 0.0702702703;
	
	color = vec4(finalColor, 1);
}