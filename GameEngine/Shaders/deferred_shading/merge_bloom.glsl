#version 330

out vec4 color;

uniform sampler2D bloom_effect;
uniform sampler2D diffuse;

in vec2 interpolated_texCoord;

void main(void)
{
  color = vec4(vec3(texture(diffuse, interpolated_texCoord).rgb), 1.0) + vec4(vec3(texture(bloom_effect, interpolated_texCoord).rgb), 1.0);
}
