
#include <Core/CullableObjects.hh>

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


}