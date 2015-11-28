#include <Render/ProgramResources/Types/Uniform/Vec2.hh>
#include <iostream>

namespace AGE
{
	Vec2::Vec2(glm::vec2 const &value, GLint id, std::string &&name) :
		IProgramResources(id, std::move(name), GL_UNIFORM),
		_value(value)
	{

	}

	Vec2::Vec2(Vec2 &&move) :
		IProgramResources(std::move(move)),
		_value(move._value)
	{

	}

	Vec2::Vec2(Vec2 const &copy) :
		IProgramResources(copy),
		_value(copy._value)
	{
	}

	Vec2 &Vec2::operator=(glm::vec2 const &p)
	{
		_update = false;
		_value = p;
		return (*this);
	}

	IProgramResources & Vec2::update()
	{
		if (!_update) {
			glUniform2f(_id, _value.x, _value.y);
			_update = true;
		}
		return (*this);
	}

	bool Vec2::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	size_t Vec2::size() const
	{
		return (sizeof(type_t));
	}

	void Vec2::print() const
	{
		std::cout << "uniform vec2 " << _name.str() << ";";
		std::cout << std::endl;
	}
}