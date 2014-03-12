#version 430 core

#define		MAX_LIGHT_NBR	255

layout (std140) uniform MaterialBasic
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 transmittance;
	vec3 emission;
	float shininess;
};

layout (std140) uniform PerFrame
{
	mat4 projection;
	mat4 view;
	float time;
	uint		pointLightNbr;
	uint		spotLightNbr;
};

layout (std140) uniform PerModel
{
	mat4 model;
};


layout(binding = 0) uniform sampler2D fTexture0; //ambient;
layout(binding = 1) uniform sampler2D fTexture1; //diffuse;
layout(binding = 2) uniform sampler2D fTexture2; //specular;
layout(binding = 3) uniform sampler2D fTexture3; //normal;

struct PointLight
{
	vec4	positionPower;
	vec4	colorRange;
	uint	hasShadow;
};

struct SpotLight
{
	mat4	lightVP;
	vec4	positionPower;
	vec4	colorRange;
	uint	hasShadow;
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

	for (uint i = 0; i < spotLightNbr; ++i)
	{
		vec4	lightPos = view * vec4(spotLights[i].positionPower.xyz, 1.0f);
		vec3	lightColor = spotLights[i].colorRange.xyz;
		float	lightRange = spotLights[i].colorRange.w;
		float	lightPower = spotLights[i].positionPower.w;

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

				spotLightsColor += factor * addedColor;
			}
		}
	}
	return (spotLightsColor);
}

void main(void)
{
	vec3	ambientColor = 0.05f * texture2D(fTexture0, fTexCoord).rgb * ambient;
	vec3	diffuseColor = texture2D(fTexture0, fTexCoord).rgb * diffuse;
	vec3	specularColor = texture2D(fTexture0, fTexCoord).rgb * specular;

	vec3	fragNormal = normalize(fNormal.xyz);

	vec4	finalColor = vec4(ambientColor, 1);
	finalColor.xyz += computePointLightsInfluence(diffuseColor, specularColor);
	finalColor.xyz += computeSpotLightsInfluence(diffuseColor, specularColor);

	FragColor = finalColor;
}