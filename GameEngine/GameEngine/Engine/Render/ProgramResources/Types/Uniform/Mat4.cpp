#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace AGE
{
	Mat4::Mat4(glm::mat4 const &value, GLint id, std::string &&name) :
		AProgramResources(id, std::move(name), GL_UNIFORM),
		_value(value)
	{

	}

	Mat4::Mat4(Mat4 &&move) :
		AProgramResources(std::move(move)),
		_value(move._value)
	{

	}

	Mat4::Mat4(Mat4 const &copy) :
		AProgramResources(copy),
		_value(copy._value)
	{

	}

	Mat4 &Mat4::operator=(glm::mat4 const &m)
	{
		_update = false;
		_value = m;
		return (*this);
	}

	IProgramResources & Mat4::update()
	{
		if (!_update) {
			glUniformMatrix4fv(_id, 1, GL_FALSE, (GLfloat *)(glm::value_ptr(_value)));
			_update = true;
		}
		return (*this);
	}

	bool Mat4::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	size_t Mat4::size() const
	{
		return (sizeof(type_t));
	}

	void Mat4::print() const
	{
		std::cout << "uniform mat4 " << _name << ";";
		std::cout << std::endl;
	}
}