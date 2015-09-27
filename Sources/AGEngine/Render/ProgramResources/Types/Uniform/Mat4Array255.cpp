#include <Render/ProgramResources/Types/Uniform/Mat4Array255.hh>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace AGE
{
	Mat4Array255::Mat4Array255(glm::mat4 *value, GLint id, std::string &&name) :
		AProgramResources(id, std::move(name), GL_UNIFORM),
		_value(value)
	{

	}

	Mat4Array255::Mat4Array255(Mat4Array255 &&move) :
		AProgramResources(std::move(move)),
		_value(move._value)
	{

	}

	Mat4Array255::Mat4Array255(Mat4Array255 const &copy) :
		AProgramResources(copy),
		_value(copy._value)
	{

	}

	Mat4Array255 &Mat4Array255::operator=(glm::mat4 *m)
	{
		_update = false;
		_value = m;
		return (*this);
	}

	IProgramResources & Mat4Array255::update()
	{
		if (!_update && _value) {
			glUniformMatrix4fv(_id, 166, GL_FALSE, (GLfloat *)(glm::value_ptr(*_value)));
			_update = true;
		}
		return (*this);
	}

	bool Mat4Array255::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	size_t Mat4Array255::size() const
	{
		return (sizeof(type_t));
	}

	void Mat4Array255::print() const
	{
		std::cout << "uniform mat4 " << _name << ";";
		std::cout << std::endl;
	}
}