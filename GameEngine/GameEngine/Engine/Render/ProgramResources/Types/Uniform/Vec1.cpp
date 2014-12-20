#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <iostream>

Vec1::Vec1(float v, GLint id, std::string &&name) :
AProgramResources(id, std::move(name), GL_UNIFORM),
ABlockResources(),
_value(v)
{

}

Vec1::Vec1(Vec1 &&move) :
AProgramResources(std::move(move)),
ABlockResources(move),
_value(move._value)
{

}

Vec1::Vec1(Vec1 const &copy) :
AProgramResources(copy),
ABlockResources(copy),
_value(copy._value)
{

}

Vec1 & Vec1::operator=(float v)
{
	_value = v;
	return (*this);
}

IProgramResources & Vec1::operator()()
{
	if (!_update) {
		glUniform1f(_id, _value);
		_update = true;
	}
	return (*this);
}

bool Vec1::safe(size_t size) const
{
	return ((sizeof(type_t) == size) ? true : false);
}

size_t Vec1::size() const
{
	return (sizeof(type_t));
}

void const * Vec1::data() const
{
	return (&_value);
}

void Vec1::print() const
{
	std::cout << "uniform float " << _name << ";";
	std::cout << std::endl;
}

