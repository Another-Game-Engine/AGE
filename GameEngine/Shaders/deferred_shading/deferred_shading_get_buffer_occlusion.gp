#version 430

uniform mat4 projection_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

uniform sampler2D diffuse_map;
uniform sampler2D normal_map;

layout(triangles) in; // triangles en entree (glDraw GL_TRIANGLES)
layout(points, max_vertices = 3) out; // points en sortie -> 1 seul point

flat in int Visible[1];

void main()
{
	if ( Visible[0] == 1 )
	{
		EmitStreamVertex(0);
	} 
}
