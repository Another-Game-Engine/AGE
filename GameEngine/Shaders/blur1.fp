#version 430
//#extension GL_ARB_separate_shader_objects: enable

// texture coordinates from vertex shaders
in vec2 st;

// framebuffer texture sampler
uniform layout (location = 0) sampler2D layer0;
uniform layout (location = 1) sampler2D layer1;

// output fragment colour RGBA
layout (location = 0) out vec4 frag_colour;

uniform float pixelOffset[10] = float[](0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
uniform float weight[10] = float[](0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);
uniform int height = 1080;

float luma(vec3 color)
{
   return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main ()
{
    float dy = 1.0 / float(height);

	vec4 sum = texture(layer1, st) * weight[0];
	for	(int i = 1; i < 10; i++)
	{
        sum += texture(layer1, st + vec2(0.0, pixelOffset[i]) * dy)	* weight[i];
        sum += texture(layer1, st - vec2(0.0, pixelOffset[i]) * dy)	* weight[i];
	}
	frag_colour	= texture(layer0, st) + sum;
}