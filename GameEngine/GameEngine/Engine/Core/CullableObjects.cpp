
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

	void Drawable::reset()
	{
		mesh = SubMeshInstance();
		transformation = glm::mat4(1);
		meshAABB = AABoundingBox();
		animation = Key<AGE::AnimationInstance>();
		this->toAddInOctree = false;
		this->hasMoved = false;
		bones.clear();
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