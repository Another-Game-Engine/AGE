#version 330

layout (std140) uniform MaterialBasic
{
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 transmittance;
  vec3 emission;
  float shininess;
};

layout (std140) uniform PerFrame
{
        mat4 projection;
        mat4 view;
        vec4 light;
        float time;
};

layout (std140) uniform PerModel
{
        mat4 model;
};

uniform          sampler2D fTexture0;//ambient;
uniform          sampler2D fTexture1;//diffuse;
uniform          sampler2D fTexture2;//specular;
uniform          sampler2D fTexture3;//normal;


in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out layout (location = 0) vec4 FragColor;
out layout (location = 1) vec4 Lol;

void main(void)
{
  FragColor = texture2D(fTexture0, fTexCoord);
  Lol = vec4(ambient.rgb, 1);
}
