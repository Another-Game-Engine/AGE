 #version 330
 #extension GL_ARB_geometry_shader4 : enable

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

in vec4 gPosition[3];
in vec4 gColor[3];
in vec4 gNormal[3];
in vec2 gTexCoord[3];

out vec4 fPosition;
out vec4 fColor;
out vec4 fNormal;
out vec2 fTexCoord;

 void main()
 {
	int		i;

   for (i = 0; i < gl_VerticesIn; i++)
   {
     gl_Position = gl_PositionIn[i];
	 fPosition = gPosition[i];
	 fColor = gColor[i];
	 fNormal = gNormal[i];
	 fTexCoord = gTexCoord[i];
     EmitVertex();
   }
   EndPrimitive();
 }