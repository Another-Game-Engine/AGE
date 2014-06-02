#version 430
#extension GL_ARB_geometry_shader4 : enable

layout(points) in;

layout(triangle_strip, max_vertices=4) out;

uniform vec3 CameraPos;

uniform mat4 View;
uniform mat4 Projection;

in vec4 MyColor[1];
out vec4 fMyColor;

out vec2 fTexCoord;

in float MyTime[1];

void main()
{
  mat4 mvp = Projection * View;
  vec3 pos = gl_PositionIn[0].xyz;

  vec3 toCamera = normalize(CameraPos - pos);
  vec3 up = vec3(0.0, 1.0, 0.0);
  vec3 right = cross(toCamera, up);

  pos -= (right * 0.5);

  gl_Position = mvp * vec4(pos, 1.0);

  fMyColor = MyColor[0];
  fTexCoord = vec2(0,0);
  EmitVertex();



  pos.y += 1.0;

  gl_Position = mvp * vec4(pos.x - cos(MyTime[0]) * sin(MyTime[0]) , pos.y, pos.z, 1.0);

  fMyColor = MyColor[0];
  fTexCoord = vec2(0,1);
  EmitVertex();



  pos.y -= 1.0;

  pos += right;

  gl_Position = mvp * vec4(pos, 1.0f);// vec4(pos , 1.0);

  fMyColor = MyColor[0];
  fTexCoord = vec2(1,0);
  EmitVertex();



  pos.y += 1.0;

  gl_Position = mvp * vec4(pos.x - cos(MyTime[0]) * sin(MyTime[0]) , pos.y, pos.z, 1.0);

  fMyColor = MyColor[0];
  fTexCoord = vec2(1,1);
  EmitVertex();



  EndPrimitive();
}



