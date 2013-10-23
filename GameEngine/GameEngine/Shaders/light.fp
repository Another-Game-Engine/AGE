#version 330

layout (std140) uniform PerFrame
{
	mat4 vProjection;
	mat4 vView;
	vec3 fLightSpot;
};

layout (std140) uniform PerModel
{
	mat4 vModel;
};

uniform	sampler2D fTexture;

in vec4 fPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out vec4 FragColor;

void main(void)
{
  /**
   * Calcule des vecteur indispensable au calcule de light :
   * - vecteur du frag au spot
   * - vecteur de reflection de la lumiére
   * - vecteur de l'oeil vers le frag
   */
  vec3 lightPos = (vView * vec4(fLightSpot, 1)).xyz;
  vec4 vectorLight = normalize(vec4(lightPos - fPosition.xyz, 1.0));
  vec4 vectorReflect = normalize(reflect(-vectorLight, fNormal));
  vec4 vectorView = normalize(vec4(fPosition.xyz - vView[3].xyz, 1.0));
  /**
   * calcule de lambert afin de determiner le cos(a)
   * entre le vecteur de lumiére et la normal du fragment.
   */
  float lamberTerm = clamp(dot(fNormal.xyz, vectorLight.xyz), 0.0, 1.0);
  vec4 pxlColor = fColor /** texture2D(fTexture, fTexCoord)*/;
  vec4 ambiant = pxlColor * vec4(0.01, 0.01, 0.01, 1.0);
  vec4 diffuse = pxlColor * lamberTerm;
  FragColor = max(ambiant, diffuse);
//	FragColor = vec4(1, 0, 0, 1);
}
