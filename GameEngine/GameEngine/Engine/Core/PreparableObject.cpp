#include <Core/PreparableObject.hh>

namespace AGE
{
	PreparableObject::PreparableObject()
		: active(true),
		hasMoved(true),
		position(0.f),
		forward(0.f),
		scale(1.0f)
	{

	}

	PreparableObject::~PreparableObject()
	{

	}

	Drawable::Drawable()
		: material(MaterialInstance::createKey())
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
		range(glm::vec3(1.0f, 0.1f, 0.01f)),
		color(glm::vec3(1.f))
	{
	}

	PointLight::PointLight(glm::vec3 const &position, glm::vec3 const &color, glm::vec3 const &range)
		: range(range),
		color(color)
	{
		PreparableObject::position = position;
	}
}