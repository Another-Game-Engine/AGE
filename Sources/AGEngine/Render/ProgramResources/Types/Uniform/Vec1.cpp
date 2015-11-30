#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <iostream>

namespace AGE
{
	Vec1::Vec1(float v, GLint id, std::string &&name) :
		IProgramResources(id, std::move(name), GL_UNIFORM),
		_value(v)
	{

	}

	Vec1::Vec1(Vec1 &&move) :
		IProgramResources(std::move(move)),
		_value(move._value)
	{

	}

	Vec1::Vec1(Vec1 const &copy) :
		IProgramResources(copy),
		_value(copy._value)
	{

	}

	Vec1 & Vec1::operator=(float v)
	{
		_update = false;
		_value = v;
		return (*this);
	}

	IProgramResources & Vec1::update()
	{
		if (!_update) {
			glUniform1f(_id, _value);
			_update = true;
		}
		return (*this);
	}

	bool Vec1::safe(size_t s) const
	{
		return ((size() == s) ? true : false);
	}

	size_t Vec1::size() const
	{
		return (sizeof(type_t));
	}

	void Vec1::print() const
	{
		std::cout << "uniform float " << _name.str() << ";";
		std::cout << std::endl;
	}

}