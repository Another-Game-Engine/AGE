#version 330

uniform sampler2D fTexture0;

in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out layout (location = 0) vec4 FragColor;

void main(void)
{
//  FragColor = vec4(1,1,1,1);//
//  FragColor = fColor;
  FragColor = vec4(1,1,1,texture2D(fTexture0, fTexCoord).a) * fColor;
}