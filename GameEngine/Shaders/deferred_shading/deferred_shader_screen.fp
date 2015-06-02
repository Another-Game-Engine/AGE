#version 330

uniform sampler2D screen;
uniform vec2 resolution;
uniform float activated;

layout (location = 0) out vec4 color;

in vec2 texCoord;

void main(void)
{
	vec2 fragCoord = texCoord * resolution;
	float FXAA_SPAN_MAX = 32.0;
    float FXAA_REDUCE_MUL = 1.0/8.0;
    float FXAA_REDUCE_MIN = 1.0/128.0;
	
	// get the pixel around
    vec3 rgbNW =texture2D(screen, fragCoord + (vec2(-1.0,-1.0) / resolution)).xyz;
    vec3 rgbNE =texture2D(screen, fragCoord + (vec2(1.0,-1.0) / resolution)).xyz;
    vec3 rgbSW =texture2D(screen, fragCoord + (vec2(-1.0,1.0) / resolution)).xyz;
    vec3 rgbSE =texture2D(screen, fragCoord + (vec2(1.0,1.0) / resolution)).xyz;
    vec3 rgbM =texture2D(screen,texCoord).xyz;
	// extract luminance
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    vec2 dir = vec2(-((lumaNW + lumaNE) - (lumaSW + lumaSE)), ((lumaNW + lumaSW) - (lumaNE + lumaSE)));
    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FXAA_REDUCE_MUL),
        FXAA_REDUCE_MIN);
    float rcpDirMin = 1.0/(min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2( FXAA_SPAN_MAX,  FXAA_SPAN_MAX),
          max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
          dir * rcpDirMin)) / resolution;
    vec3 rgbA = (1.0f/2.0f) * (
        texture2D(screen, texCoord + dir * (1.0f/3.0f - 0.5f)).xyz +
        texture2D(screen, texCoord + dir * (2.0f/3.0f - 0.5f)).xyz);
    vec3 rgbB = rgbA * (1.0f/2.0f) + (1.0f/4.0f) * (
        texture2D(screen, texCoord + dir * -0.5f).xyz +
        texture2D(screen, texCoord + dir * 0.5f).xyz);
    float lumaB = dot(rgbB, luma);
    if ((lumaB < lumaMin) || (lumaB > lumaMax)){
        color = mix(texture(screen, texCoord), vec4(rgbA, 1.0f), activated);
    }else{
		color = mix(texture(screen, texCoord), vec4(rgbB, 1.0f), activated);
    }
}