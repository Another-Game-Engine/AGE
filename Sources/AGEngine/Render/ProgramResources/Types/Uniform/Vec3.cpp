#include <Render/ProgramResources/Types/Uniform/Vec3.hh>
#include <iostream>

namespace AGE
{
	Vec3::Vec3(glm::vec3 const &value, GLint id, std::string &&name) :
		AProgramResources(id, std::move(name), GL_UNIFORM),
		_value(value)
	{

	}

	Vec3::Vec3(Vec3 &&move) :
		AProgramResources(std::move(move)),
		_value(move._value)
	{

	}

	Vec3::Vec3(Vec3 const &copy) :
		AProgramResources(copy),
		_value(copy._value)
	{
	}

	Vec3 &Vec3::operator=(glm::vec3 const &p)
	{
		_update = false;
		_value = p;
		return (*this);
	}

	IProgramResources & Vec3::update()
	{
		if (!_update) {
			glUniform3f(_id, _value.x, _value.y, _value.z);
			_update = true;
		}
		return (*this);
	}

	bool Vec3::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	size_t Vec3::size() const
	{
		return (sizeof(type_t));
	}

	void Vec3::print() const
	{
		std::cout << "uniform vec3 " << _name.str() << ";";
		std::cout << std::endl;
	}
}