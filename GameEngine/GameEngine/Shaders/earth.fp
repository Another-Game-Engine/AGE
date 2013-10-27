#version 330

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

uniform	sampler2D fDayTexture;
uniform	sampler2D fNightTexture;
uniform	sampler2D fClouds;
uniform	sampler2D fBump;

in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out vec4 FragColor;

void main(void)
{
  vec4 normal = vec4(normalize(fNormal.xyz + texture2D(fBump, fTexCoord).xyz), 0);
  /**
   * Calcule des vecteur indispensable au calcule de light :
   * - vecteur du frag au spot
   * - vecteur de reflection de la lumiére
   * - vecteur de l'oeil vers le frag
   */
  vec3 lightPos = (view * light).xyz;
  vec4 vectorLight = normalize(vec4(lightPos - fPosition.xyz, 1.0));
  vec4 vectorReflect = normalize(reflect(-vectorLight, normal));
  vec4 vectorView = normalize(vec4(fPosition.xyz - view[3].xyz, 1.0));
  /**
   * calcule de lambert afin de determiner le cos(a)
   * entre le vecteur de lumiére et la normal du fragment.
   */
  float lamberTerm = clamp(dot(normal.xyz, vectorLight.xyz), 0.03, 1.0);
  FragColor = lamberTerm * texture2D(fDayTexture, fTexCoord) +
				(1.0 - lamberTerm) * texture2D(fNightTexture, fTexCoord) +
				lamberTerm * texture2D(fClouds, fTexCoord);
//	FragColor = vec4(lamberTerm, lamberTerm, lamberTerm, 1);
}
