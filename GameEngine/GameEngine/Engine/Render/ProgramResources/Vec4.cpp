#pragma once

#include <Render/ProgramResources/Vec4.hh>
#include <Render/Program.hh>

Vec4::Vec4(glm::vec4 const &value, Program const &parent, std::string const &name) :
AProgramResources(parent, name)
{

}

Vec4::Vec4(glm::vec4 const &value, Program const &parent, std::string &&name) :
AProgramResources(parent, name),
_value(value)
{

}

Vec4::Vec4(Vec4 &&move):
AProgramResources(std::move(move)),
_value(move._value)
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
IProgramResource & Vec4::operator()()
{
	if (!_isUpdate) {
		glUniform4f(_id, _value.x, _value.y, _value.z, _value.w);
		_isUpdate = true;
	}
	return (*this);
}

