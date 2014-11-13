#include <Render/ResourceProgram.hh>
#include <Render/Program.hh>
#include <Utils/OpenGL.hh>
#include <utility>


IResourceProgram::~IResourceProgram()
{

}


AResourceProgram::AResourceProgram(Program const &parent, std::string const &name) :
_parent(parent),
_name(name),
_isUpdate(false)
{
	assert(create());
}

AResourceProgram::AResourceProgram(Program const &parent, std::string &&name) :
_parent(parent),
_name(std::move(name)),
_isUpdate(false)
{
	assert(create());
}

AResourceProgram::~AResourceProgram()
{

}

bool AResourceProgram::create()
{
	_id = glGetUniformLocation(_parent.getId(), _name.c_str());
	return ((_id > -1) ? true : false);
}

bool AResourceProgram::isUpdate() const
{
	return (_isUpdate);
}

void AResourceProgram::setUpdate(bool isUpdate)
{
	_isUpdate = isUpdate;
}

Vec4::Vec4(glm::vec4 const &value, Program const &parent, std::string const &name) :
AResourceProgram(parent, name),
_value(value)
{
}

Vec4::Vec4(glm::vec4 const &value, Program const &parent, std::string &&name):
AResourceProgram(parent, std::move(name)),
_value(value)
{

}

Vec4 & Vec4::operator=(glm::vec4 const &value)
{
	_value = value;
	return (*this);
}

IResourceProgram & Vec4::operator()()
{
	glUniform4f(_parent.getId(), _value.x, _value.y, _value.z, _value.w);
	return (*this);
}

Mat4::Mat4(glm::mat4 const &value, Program const &parent, std::string const &name) :
AResourceProgram(parent, name),
_value(value)
{

}

Mat4::Mat4(glm::mat4 const &value, Program const &parent, std::string &&name):
AResourceProgram(parent, std::move(name)),
_value(value)
{

}

Mat4 & Mat4::operator=(glm::mat4 const &value)
{
	_value = value;
	return (*this);
}

IResourceProgram & Mat4::operator()()
{
	glUniformMatrix4fv(_id, 1, GL_FALSE, (GLfloat *)(&_value[0]));
	return (*this);
}
