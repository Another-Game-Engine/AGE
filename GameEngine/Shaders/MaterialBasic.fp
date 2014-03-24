#version 430 core

#define		MAX_LIGHT_NBR	255

layout (shared) uniform MaterialBasic
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 transmittance;
	vec3 emission;
	float shininess;
};

layout (shared) uniform PerFrame
{
	mat4		projection;
	mat4		view;
	float		time;
	uint		pointLightNbr;
	uint		spotLightNbr;
};

layout (shared) uniform PerModel
{
	mat4 model;
};


layout(binding = 0) uniform sampler2D fTexture0; //ambient;
layout(binding = 1) uniform sampler2D fTexture1; //diffuse;
layout(binding = 2) uniform sampler2D fTexture2; //specular;
layout(binding = 3) uniform sampler2D fTexture3; //normal;

// Shadow map
layout(binding = 4) uniform sampler2DArrayShadow spotShadowMaps;

struct PointLight
{
	vec4	positionPower;
	vec4	colorRange;
	int		shadowId;
};

struct SpotLight
{
	mat4	lightVP;
	vec4	positionPower;
	vec4	colorRange;
	int		shadowId;
};

layout(std140) uniform pointLightBuff
{
	PointLight	pointLights[MAX_LIGHT_NBR];
};

layout(std140) uniform spotLightBuff
{
	SpotLight	spotLights[MAX_LIGHT_NBR];
};

in vec4 fScreenPosition;
in vec4 fWorldPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out layout (location = 0) vec4 FragColor;

float random_number(vec3 seed, int index)
{
	vec4 seed4 = vec4(seed, index);
	float dot_product = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return fract(sin(dot_product) * 43758.5453);
}

float		calcSpecular(vec3 lightPos, vec3 fragToLight)
{
	vec3	eyePos = normalize(-fScreenPosition.xyz);
	vec3	lightReflection = normalize(-reflect(fragToLight, normalize(fNormal.xyz)));

	return clamp(pow(max(dot(lightReflection, eyePos), 0.0), 0.3f * shininess), 0.0f, 1.0f);
}

vec3		computePointLightsInfluence(vec3 diffuseColor, vec3 specularColor)
{
	vec3	pointLightsColor = vec3(0);

	for (uint i = 0; i < pointLightNbr; ++i)
	{
		vec4	lightPos = view * vec4(pointLights[i].positionPower.xyz, 1.0f);
		vec3	lightColor = pointLights[i].colorRange.xyz;
		float	lightRange = pointLights[i].colorRange.w;
		float	lightPower = pointLights[i].positionPower.w;
	
		vec3	fragToLight = lightPos.xyz - fScreenPosition.xyz;
		float	fragToLightDist = length(fragToLight);
	
		if (fragToLightDist < lightRange) // Ugly test waiting for tile based forward rendering
		{
			fragToLight = normalize(fragToLight);
	
			float	illumination =  clamp(dot(fragToLight, normalize(fNormal.xyz)), 0.0f, 1.0f);
			float	specular = calcSpecular(lightPos.xyz, fragToLight);
			float	diminution = clamp(1.0f - fragToLightDist / lightRange, 0.0f, 1.0f);

			pointLightsColor += lightPower * specular * diminution * lightColor * specularColor;
			pointLightsColor += lightPower * illumination * diminution * lightColor * diffuseColor;
		}
	}
	return (pointLightsColor);
}

vec3		computeSpotLightsInfluence(vec3 diffuseColor, vec3 specularColor)
{
	vec3	spotLightsColor = vec3(0);
	vec2	poissonDisk[4] = vec2[](	vec2(-0.94201624, -0.39906216),
										vec2(0.94558609, -0.76890725),
										vec2(-0.094184101, -0.92938870),
										vec2(0.34495938, 0.29387760)
									);

	for (uint i = 0; i < spotLightNbr; ++i)
	{
		vec4	lightPos = view * vec4(spotLights[i].positionPower.xyz, 1.0f);
		vec3	lightColor = spotLights[i].colorRange.xyz;
		float	lightRange = spotLights[i].colorRange.w;
		float	lightPower = spotLights[i].positionPower.w;
		int		shadowIndex = spotLights[i].shadowId;

		// Calculate the influence factor depending on the frustum
		vec4	shadowPosition = spotLights[i].lightVP * fWorldPosition;
		vec4	projectedPosition = shadowPosition / shadowPosition.w;

		if (shadowPosition.z > 0)
		{
			float factor = clamp(1.0f - abs(projectedPosition.x), 0.0f, 1.0f) *
					 clamp(1.0f - abs(projectedPosition.y), 0.0f, 1.0f);

			vec3	fragToLight = lightPos.xyz - fScreenPosition.xyz;
			float	fragToLightDist = length(fragToLight);

			if (fragToLightDist < lightRange) // Ugly test waiting for tile based forward rendering
			{
				fragToLight = normalize(fragToLight);
	
				float	illumination =  clamp(dot(fragToLight, normalize(fNormal.xyz)), 0.0f, 1.0f);
				float	specular = calcSpecular(lightPos.xyz, fragToLight);
				float	diminution = clamp(1.0f - fragToLightDist / lightRange, 0.0f, 1.0f);
	
				vec3	addedColor = lightPower * specular * diminution * lightColor * specularColor;
				addedColor += lightPower * illumination * diminution * lightColor * diffuseColor;

				ivec3	shadowSize = textureSize(spotShadowMaps, 0);
				float	shadowRatio = 1.0f;
				float	bias = 0.0001f;

				if (shadowIndex != -1)
				{
					vec2		diskSpreading = vec2(shadowSize.xy);

					for (int i = 0; i < 4; ++i)
					{
						int index = int(4.0 * random_number(floor(fWorldPosition.xyz * 1000.0), i)) % 4;
						float	shadowTest = texture(spotShadowMaps, vec4(projectedPosition.xy * vec2(0.5f) + vec2(0.5f) + vec2(poissonDisk[index]) / diskSpreading,
																	float(shadowIndex),
																	projectedPosition.z * 0.5f + 0.5f - bias));
						shadowRatio -=  (1.0f - shadowTest) / 4.0f;
					}
				}

				spotLightsColor += shadowRatio * factor * addedColor;
			}
		}
	}
	return (spotLightsColor);
}

void main(void)
{
	vec3	ambientColor = 0.05f * texture2D(fTexture0, fTexCoord).rgb * ambient;
	vec4	diffuseColor = texture2D(fTexture1, fTexCoord) * vec4(diffuse, 1);
	vec3	specularColor = texture2D(fTexture2, fTexCoord).rgb * specular;

	if (diffuseColor.a == 0)
		discard;

	vec4	finalColor = vec4(ambientColor, 1);
	finalColor.xyz += computePointLightsInfluence(diffuseColor.xyz, specularColor);
	finalColor.xyz += computeSpotLightsInfluence(diffuseColor.xyz, specularColor);

	FragColor = finalColor;
}