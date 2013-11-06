#version 330

// framebuffer texture sampler
uniform layout (location = 1) sampler2D layer1;
uniform layout (location = 2) sampler2D layer2;
uniform layout (location = 3) sampler2D layer3;

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

uniform	sampler2D fTexture0;
uniform	sampler2D fTexture1;

in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out layout (location = 0) vec4 FragColor;

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
  float lamberTerm = clamp(dot(bNormal.xyz, vectorLight.xyz), 0.0, 1.0);
  vec4 pxlColor = fColor * texture2D(fTexture0, fTexCoord);
  vec4 ambiant = pxlColor * vec4(0.05, 0.05, 0.05, 1.0);
  vec4 diffuse = pxlColor * lamberTerm;
  FragColor = max(ambiant, diffuse);
}
