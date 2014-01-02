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
  vec4 bNormal = vec4(normalize(fNormal.xyz + texture2D(fTexture1, fTexCoord).xyz), 0);
  /**
   * Calcule des vecteur indispensable au calcule de light :
   * - vecteur du frag au spot
   * - vecteur de reflection de la lumiére
   * - vecteur de l'oeil vers le frag
   */
  vec3 lightPos = (view * light).xyz;
  vec4 vectorLight = normalize(vec4(lightPos - fPosition.xyz, 1.0));
  vec4 vectorReflect = normalize(reflect(-vectorLight, bNormal));
  vec4 vectorView = normalize(vec4(fPosition.xyz - view[3].xyz, 1.0));
  /**
   * calcule de lambert afin de determiner le cos(a)
   * entre le vecteur de lumiére et la normal du fragment.
   */
  float lamberTerm = clamp(dot(diffuse.xyz, vectorLight.xyz), 0.0, 1.0);
  vec4 pxlColor = fColor * texture2D(fTexture0, fTexCoord);
  vec4 cAmbient = pxlColor * vec4(ambient.rgb, 1.0);
  vec4 cDiffuse = pxlColor * lamberTerm;
  FragColor = texture2D(fTexture1, fTexCoord);//max(cAmbient, cDiffuse);
}
