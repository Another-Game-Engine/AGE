#version 330

uniform sampler2D fTexture0;
uniform mat4 projection;
uniform mat4 transformation;
uniform vec4 color;

in vec4 fPosition;
in vec4 fColor;
in vec2 fTexCoord;

out layout (location = 0) vec4 FragColor;

void main(void)
{
  float opacity = texture2D(fTexture0, fTexCoord).a;
  if (opacity == 0)
    discard;
  FragColor = vec4(1,1,1, opacity) * fColor;
}