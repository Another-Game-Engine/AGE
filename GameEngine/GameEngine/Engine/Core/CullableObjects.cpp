
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

	Drawable::Drawable(SubMeshInstance &&_m, MaterialInstance &&_mat, glm::mat4 &&_t)
		: mesh(std::move(_m))
		, material(std::move(_mat))
		, transformation(std::move(_t))
	{
	}

	PointLight::PointLight()
		: PreparableObject(),
		power(1.0f),
		range(1.0f),
		color(1.0f)
	{
	}

	PointLight::PointLight(float power, float range, glm::vec3 const &color, glm::vec3 const &position)
		: power(power),
		range(range),
		color(color)
	{
		PreparableObject::position = position;
	}


}