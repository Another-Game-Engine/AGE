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

	uvec3	padding;
};

struct SpotLight
{
	mat4	lightVP;
	vec4	positionPower;
	vec4	colorRange;
	uint	hasShadow;

	uvec3	padding;
};

layout(std140) uniform pointLightBuff
{
	PointLight	pointLights[MAX_LIGHT_NBR];
	uint		pointLightNbr;
};

layout(std140) uniform spotLightBuff
{
	SpotLight	spotLights[MAX_LIGHT_NBR];
	uint		spotLightNbr;
};

in vec4 fScreenPosition;
in vec4 fWorldPosition;
in vec4 fColor;
in vec4 fNormal;
in vec2 fTexCoord;

out layout (location = 0) vec4 FragColor;

float		calcSpecular(vec3 lightPos, vec3 fragToLight)
{
	vec3	eyePos = normalize(-fPosition.xyz);
	vec3	lightReflection = normalize(-reflect(fragToLight, normalize(fNormal.xyz)));

	return clamp(pow(max(dot(lightReflection, eyePos), 0.0), 0.3f * shininess), 0.0f, 1.0f);
}

void main(void)
{
	vec3	ambientColor = 0.05f * texture2D(fTexture0, fTexCoord).rgb * ambient;
	vec3	diffuseColor = texture2D(fTexture0, fTexCoord).rgb * diffuse;
	vec3	specularColor = texture2D(fTexture0, fTexCoord).rgb * specular;

	// TODO: Implement a real normal mapping: this is an ugly trick just to see if it workss
	vec3	fragNormal = normalize(normalize(fNormal.xyz) + normalize(texture2D(fTexture3, fTexCoord).xyz));

	vec4	finalColor = vec4(ambientColor, 1);

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
	
			finalColor.xyz += lightPower * specular * diminution * lightColor * specularColor;
			finalColor.xyz += lightPower * illumination * diminution * lightColor * diffuseColor;
		}
	}

	for (uint i = 0; i < spotLightNbr; ++i)
	{
		vec4	lightPos = view * vec4(spotLights[i].positionPower.xyz, 1.0f);
		vec3	lightColor = spotLights[i].colorRange.xyz;
		float	lightRange = spotLights[i].colorRange.w;
		float	lightPower = spotLights[i].positionPower.w;

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

			// Calculate the influence factor depending on the frustum
			float	factor;
			vec4	shadowPosition = spotLights[i].lightVP * fWorldPosition;
			vec4	projectedPosition = shadowPosition / shadowPosition.w;

			if (projectedPosition.x < 0.5f)
				factor = projectedPosition.x * 2.0f;
			else
				factor = 1.0f - (projectedPosition.x - 0.5f) * 2.0f;
			if (projectedPosition.y < 0.5f)
				factor *= projectedPosition.y * 2.0f;
			else
				factor *= 1.0f - (projectedPosition.y - 0.5f) * 2.0f;
			//--------------------------------------------------------
			finalColor.xyz += factor * addedColor;
		}
	}

	FragColor = finalColor;
}