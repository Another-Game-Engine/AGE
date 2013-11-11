#version 330

uniform float roll;
uniform	image2D	destTex;
layout (local_size_x = 16, local_size_y	= 16) in;
void main()	{
    ivec2 storePos = ivec2(gl_GlobalInvocationID.xy);
	float localCoef	= length(vec2(ivec2(gl_LocalInvocationID.xy)-8)/8.0);
	float globalCoef = sin(float(gl_WorkGroupID.x+gl_WorkGroupID.y)*0.1	+ roll)*0.5;
	imageStore(destTex,	storePos, vec4(1.0-globalCoef*localCoef, 0.0, 0.0, 0.0));
}