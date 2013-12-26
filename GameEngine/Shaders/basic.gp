 #version 330
 #extension GL_ARB_geometry_shader4 : enable

layout(triangles) in;
layout(triangle_strip, max_vertices=6) out;

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

in vec4 gColor[3];
in vec2 gUv[3];

out vec4 fColor;
out vec2 fUv;

void main()
{
int		i;

  for (i = 0; i < gl_VerticesIn; i++)
  {
    gl_Position = projection * view * model * gl_PositionIn[i];
	fColor = gColor[i];
	fUv = gUv[i];
    EmitVertex();
  }
  EndPrimitive();
}