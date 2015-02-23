#include <SpacePartitioning/Cullable/Object/Mesh.hh>

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

}