#pragma once

#include <Render/ProgramResources/Vec4.hh>
#include <Render/Program.hh>


Vec4::Vec4(glm::vec4 const &value, Program const &parent, std::string &&name) :
AProgramResources(parent, std::move(name), GL_UNIFORM),
ABlockResources(),
_value(value)
{

}

Vec4::Vec4(Vec4 &&move):
AProgramResources(std::move(move)),
ABlockResources(move),
_value(move._value)
{

}

Vec4::Vec4(glm::vec4 const &value, Program const &parent, GLint id) :
AProgramResources(parent, id,  GL_UNIFORM),
ABlockResources(),
_value(value)
{
}

IProgramResources & Vec4::operator()()
{
	if (!_update) {
		glUniform4f(_id, _value.x, _value.y, _value.z, _value.w);
		_update = true;
	}
	return (*this);
}

bool Vec4::safe(size_t size) const
{
	return ((sizeof(type) == size) ? true : false);
}

size_t Vec4::size() const
{
	return (sizeof(glm::vec4));
}

void const *Vec4::data() const
{
	return (&_value);
}