
#include <Core/CullableObjects.hh>

namespace AGE
{

	Drawable::Drawable() :
		meshAABB(glm::vec3(-1), glm::vec3(1))
	{
	}

	Drawable::~Drawable()
	{
	}

	Drawable::Drawable(const SubMeshInstance &_m, const MaterialInstance &_mat, const glm::mat4 &_t)
		: mesh(_m)
		, material(_mat)
		, transformation(_t)
	{
	}

	Drawable::Drawable(const SubMeshInstance &_m, const MaterialInstance &_mat, const glm::mat4 &_t, const std::vector<glm::mat4> &_bones)
		: mesh(_m)
		, material(_mat)
		, transformation(_t)
		, bones(_bones)
	{
	}

	PointLight::PointLight() :
		range(1.0f),
		color(1.0f)
	{
	}

	PointLight::PointLight(glm::vec3 const &pPosition, glm::vec3 const &pColor, glm::vec3 const &pRange) :
		range(pRange),
		color(pColor)
	{
		PreparableObject::position = pPosition;
	}


}