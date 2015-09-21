#include <Render/ProgramResources/Types/Uniform/Vec4.hh>
#include <iostream>

namespace AGE
{
	Vec4::Vec4(glm::vec4 const &value, GLint id, std::string &&name) :
		AProgramResources(id, std::move(name), GL_UNIFORM),
		_value(value)
	{

	}

	Vec4::Vec4(Vec4 &&move) :
		AProgramResources(std::move(move)),
		_value(move._value)
	{

	}

	Vec4::Vec4(Vec4 const &copy) :
		AProgramResources(copy),
		_value(copy._value)
	{
	}

	Vec4 &Vec4::operator=(glm::vec4 const &p)
	{
		_update = false;
		_value = p;
		return (*this);
	}

	IProgramResources & Vec4::update()
	{
		if (!_update) {
			glUniform4f(_id, _value.x, _value.y, _value.z, _value.w);
			_update = true;
		}
		return (*this);
	}

	bool Vec4::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	size_t Vec4::size() const
	{
		return (sizeof(type_t));
	}

	void Vec4::print() const
	{
		std::cout << "uniform vec4 " << _name << ";";
		std::cout << std::endl;
	}
}