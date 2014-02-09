#version 330

layout (location = 0) out float fDepth;

void main()
{

	fDepth = gl_FragCoord.z;
}