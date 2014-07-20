#include <Core/Drawable.hh>

namespace AGE
{
	Drawable::Drawable()
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
		: mesh(_m)
		, material(_mat)
		, transformation(_t)
	{
	}

	Drawable &Drawable::operator=(Drawable const &d)
	{
		mesh = d.mesh;
		material = d.material;
		transformation = d.transformation;
		return (*this);
	}

	Drawable::Drawable(Drawable const &d)
	{
		mesh = d.mesh;
		material = d.material;
		transformation = d.transformation;
	}
}