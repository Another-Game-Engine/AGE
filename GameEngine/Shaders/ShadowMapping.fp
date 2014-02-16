#version 330

layout (std140) uniform Light
{
	mat4 lightMVP;
};

layout (location = 0) out vec4 fDepth;

void main()
{
	fDepth = vec4(0.0, 1.0, 0.0, 1.0)/*gl_FragCoord.z*/;
}