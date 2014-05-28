#version 430

out vec4 outColor;
layout (location=1) in vec4 inCol;

void main()
{
	outColor = vec4(1,0,1,1);
//	outColor = inCol;
}