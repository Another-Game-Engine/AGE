
#include <Core/CullableObjects.hh>
#include <Utils/MathematicTools.hh>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace AGE
{

	Drawable::Drawable()
	{
	}

	Drawable::~Drawable()
	{
	}

	Drawable::Drawable(const SubMeshInstance &_m, const glm::mat4 &_t)
		: mesh(_m)
		, transformation(_t)
	{
	}

	Drawable::Drawable(const SubMeshInstance &_m, const glm::mat4 &_t, const std::vector<glm::mat4> &_bones)
		: mesh(_m)
		, transformation(_t)
		, bones(_bones)
	{
	}

	PointLight::PointLight() :
		attenuation(1.0f),
		color(1.0f)
	{
	}

	PointLight::PointLight(glm::vec3 const &pPosition, glm::vec3 const &pColor, glm::vec3 const &pAttenuation) :
		attenuation(pAttenuation),
		color(pColor)
	{
		PreparableObject::position = pPosition;
	}

	void PointLight::computeSphereTransform()
	{
		glm::vec3 lightRange;
		glm::vec3 equation(attenuation);

		// if the value of attenuation.x + attenuation.y * dist + attenuation.z * dist * dist == 256
		// then the pixels are not lighted anymore (pxlColor = final_color / attenuation)
		equation.x -= 256;
		if (attenuation.z == 0) // first degree
		{
			// infinite range if theire is no distance attenuation
			assert(attenuation.y != 0);
			lightRange = glm::vec3(-equation.x / equation.y);
		}
		else // second degree
		{
			float d = Mathematic::secondDegreeDiscriminant(equation);
			glm::vec2 res = Mathematic::resolveSecondDegree(attenuation, d);
			lightRange = glm::vec3(glm::max(res.x, res.y));
		}
		assert(lightRange.x > 0);
		sphereTransform = glm::scale(glm::translate(glm::mat4(1), position), lightRange);
	}
}