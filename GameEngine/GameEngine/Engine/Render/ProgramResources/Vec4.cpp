#pragma once

#include <Render/ProgramResources/Vec4.hh>
#include <Render/Program.hh>

Vec4::Vec4(glm::vec4 const &value, Program const &parent, std::string const &name) :
AProgramResources(parent, name, GL_UNIFORM),
ABlockResources()
{

}

Vec4::Vec4(glm::vec4 const &value, Program const &parent, std::string &&name) :
AProgramResources(parent, name, GL_UNIFORM),
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

Vec4::Vec4(glm::vec4 const &value, GLint id, std::string &&name) :
AProgramResources(id, std::move(name), GL_UNIFORM),
ABlockResources(),
_value(value)
{

}

/**
* Method:    operator()
* FullName:  Vec4::operator()
* Access:    virtual public 
* Returns:   IProgramResource &
* Qualifier:
* Goal:		 send data to GPU
*/
IProgramResources & Vec4::operator()()
{
	if (!_update) {
		glUniform4f(_id, _value.x, _value.y, _value.z, _value.w);
		_update = true;
	}
	return (*this);
}

/**
* Method:    safe
* FullName:  Vec4::safe
* Access:    virtual public 
* Returns:   bool
* Qualifier: const
* Parameter: size_t size
* Goal:		 Valid if the memory size in input is the same than in intern
*/
bool Vec4::safe(size_t size) const
{
	return ((sizeof(type) == size) ? true : false);
}

/**
* Method:    size
* FullName:  Vec4::size
* Access:    virtual public 
* Returns:   size_t
* Qualifier: const
* Goal:		 data size of the resource
*/
size_t Vec4::size() const
{
	return (sizeof(glm::vec4));
}

/**
* Method:    data
* FullName:  Vec4::data
* Access:    virtual public 
* Returns:   void const *
* Qualifier: const
* Goal:		 data of the vec4
*/
void const *Vec4::data() const
{
	return (&_value);
}