#include <Core/Drawable.hh>

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

	Drawable::Drawable(SubMeshInstance &&_m, glm::mat4 &&_t)
		: mesh(_m)
		, transformation(_t)
	{
	}

	Drawable &Drawable::operator=(Drawable const &d)
	{
		mesh = d.mesh;
		transformation = d.transformation;
		return (*this);
	}
}