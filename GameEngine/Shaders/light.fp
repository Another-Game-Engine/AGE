#version 330
#extension GL_ARB_explicit_uniform_location: enable
#extension GL_ARB_separate_shader_objects: enable

// framebuffer texture sampler
uniform layout (location = 0) sampler2D layer0;
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

in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out layout (location = 0) vec4 FragColor;

void main(void)
{
  /**
   * Calcule des vecteur indispensable au calcule de light :
   * - vecteur du frag au spot
   * - vecteur de reflection de la lumiére
   * - vecteur de l'oeil vers le frag
   */
  vec3 lightPos = (view * light).xyz;
  vec4 vectorLight = normalize(vec4(lightPos - fPosition.xyz, 1.0));
  vec4 vectorReflect = normalize(reflect(-vectorLight, fNormal));
  vec4 vectorView = normalize(vec4(fPosition.xyz - view[3].xyz, 1.0));
  /**
   * calcule de lambert afin de determiner le cos(a)
   * entre le vecteur de lumiére et la normal du fragment.
   */
  float lamberTerm = clamp(dot(fNormal.xyz, vectorLight.xyz), 0.0, 1.0);
  vec4 pxlColor = fColor * texture2D(fTexture0, fTexCoord);
  vec4 ambiant = pxlColor * vec4(0.01, 0.01, 0.01, 1.0);
  vec4 diffuse = pxlColor * lamberTerm;
  FragColor = max(ambiant, diffuse);
}
