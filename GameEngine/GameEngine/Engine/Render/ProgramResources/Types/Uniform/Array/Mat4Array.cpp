#include <Render/ProgramResources/Types/Uniform/Array/Mat4Array.hh>
#include <iostream>

namespace AGE
{

	Mat4Array::Mat4Array(GLint id, size_t size, size_t stride, std::string &&name) :
		AProgramResources(id, std::move(name), GL_UNIFORM),
		_stride(stride),
		_value(size)
	{
	}

	Mat4Array::Mat4Array(Mat4Array &&move) :
		AProgramResources(std::move(move)),
		_stride(move._stride),
		_value(std::move(move._value))
	{
	}

	Mat4Array::Mat4Array(Mat4Array const &copy) :
		AProgramResources(copy),
		_stride(copy._stride),
		_value(copy._value)
	{
	}

	Mat4Array &Mat4Array::operator=(std::vector<glm::mat4> const &m)
	{
		assert(m.size() == _value.size());
		_update = false;
		_value = m;
		return *this;
	}

	IProgramResources & Mat4Array::update()
	{
		if (!_update) {
			glUniformMatrix4fv(_id, _value.size(), GL_FALSE, (GLfloat *)(_value.data()));
			_update = true;
		}
		return (*this);
	}

	bool Mat4Array::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	size_t Mat4Array::size() const
	{
		return (sizeof(type_t));
	}

	void Mat4Array::print() const
	{
		std::cout << "uniform mat4 " << _name << ";";
		std::cout << std::endl;
	}
}