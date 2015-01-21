
#include <Core/CullableObjects.hh>

namespace AGE
{

	Drawable::Drawable() :
		meshAABB(glm::vec3(-1), glm::vec3(1))
	{
		active = false;
	}

	Drawable::~Drawable()
	{
	}

	Drawable::Drawable(const SubMeshInstance &_m, const MaterialInstance &_mat, const glm::mat4 &_t)
		: mesh(_m)
		, material(_mat)
		, transformation(_t)
	{
		active = false;
	}

	Drawable::Drawable(const SubMeshInstance &_m, const MaterialInstance &_mat, const glm::mat4 &_t, const std::vector<glm::mat4> &_bones)
		: mesh(_m)
		, material(_mat)
		, transformation(_t)
		, bones(_bones)
	{
		active = false;
	}

	void Drawable::reset()
	{
		mesh = SubMeshInstance();
		material = MaterialInstance();
		transformation = glm::mat4(1);
		meshAABB = AABoundingBox();
		animation = gl::Key<AGE::AnimationInstance>();
		hasMoved = false;
		active = false;
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