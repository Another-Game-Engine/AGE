#version 330
#extension GL_ARB_explicit_uniform_location: enable
#extension GL_ARB_separate_shader_objects: enable
// texture coordinates from vertex shaders
in vec2 st;

// framebuffer texture sampler
uniform layout (location = 0) sampler2D layer0;
uniform layout (location = 1) sampler2D layer1;
uniform layout (location = 2) sampler2D layer2;
uniform layout (location = 3) sampler2D layer3;

// output fragment colour RGBA
layout (location = 0) out vec4 frag_colour;

uniform float threshold = 0.3;

float luma(vec3 color)
{
   return 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
}

void main ()
{
	frag_colour	= vec4(0);//clamp(luma(vec3(texture(layer0, st))) - threshold, 0.0, 1.0) * (1.0 / (1.0 - threshold));
}