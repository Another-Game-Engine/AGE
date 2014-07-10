#include <OpenGL/Drawable.hh>

namespace gl
{
	Drawable::Drawable()
	{

	}

	Drawable::~Drawable()
	{

	}

	glm::mat4 const &Drawable::getModel() const
	{
		return (_model);
	}

	Drawable &Drawable::setModel(glm::mat4 const &m)
	{
		_model = m;
		return (*this);
	}
}