#version 400

#define FXAA_PC 1 
#define FXAA_GLSL_130 1 
#define FXAA_QUALITY__PRESET 39
#define FXAA_GREEN_AS_LUMA 1

#include(Fxaa3_11.h)

uniform sampler2D screen;
uniform vec2 resolution;
uniform float activated;

layout (location = 0) out vec4 color;

in vec2 texCoord;

void main(void)
{
	vec4 color_input = texture(screen, texCoord);
	// tone mapping operator
	color_input = color_input / (color_input + 1);
	color = mix(color_input, FxaaPixelShader(texCoord, vec4(0.f), screen, screen, screen, 1.f / resolution, vec4(0.f), vec4(-2 / resolution.x, -2 / resolution.y, 2 / resolution.x, 2 / resolution.y), vec4(0.f), float(0.75f), float(0.125f), float(0.0833), float(4.0f), float(0.125f), float(0.05f), vec4(0.f)), activated);
}