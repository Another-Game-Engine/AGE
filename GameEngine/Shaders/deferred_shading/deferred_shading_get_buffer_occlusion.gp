#version 430

layout(triangles) in;
layout (triangle_strip, max_vertices=3) out;
 
in VertexData {
	vec3 inter_normal;
	vec3 inter_tangent;
	vec2 inter_texCoord;
} VertexIn[3];
 
out VertexData {
	vec3 inter_normal;
	vec3 inter_tangent;
	vec2 inter_texCoord;
} VertexOut;

flat in int Visible[1];
	
void main()
{
	if ( Visible[0] == 1 )
	{
		for(int i = 0; i < gl_in.length(); i++)
		{
			// copy attributes
			gl_Position = gl_in[i].gl_Position;
			VertexOut.inter_normal = VertexIn[i].inter_normal;
			VertexOut.inter_texCoord = VertexIn[i].inter_texCoord;
			VertexOut.inter_tangent = VertexIn[i].inter_tangent;
 
			// done with the vertex
			EmitVertex();
		}
		EndPrimitive();
	}
}
