#version 430 core


in vec4 fMyColor;
out vec4 outColor;

in vec2 fTexCoord;

layout(binding = 0) uniform sampler2D fTexture0;

void main()
{
	outColor = texture2D(fTexture0, fTexCoord);
	if (outColor.x == 0 && outColor.y == 0 && outColor.z == 0)
	{
		discard;
	}
//	outColor = fMyColor;
}