#version 330

layout (std140) uniform Light
{
	mat4 lightMVP;
};

layout (location = 0) out float fDepth;

void main()
{
	fDepth = gl_FragCoord.z;
}