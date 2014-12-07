#include <Render/ProgramResources/Mat4.hh>
#include <Render/Program.hh>

Mat4::Mat4(glm::mat4 const &value, Program const &parent, std::string const &name) :
AProgramResources(parent, name, GL_UNIFORM),
ABlockResources(),
_value(value)
{

}

Mat4::Mat4(glm::mat4 const &value, Program const &parent, GLint id) :
AProgramResources(parent, id, GL_UNIFORM),
ABlockResources(),
_value(value)
{

}

Mat4::Mat4(glm::mat4 const &value, Program const &parent, std::string &&name):
AProgramResources(parent, std::move(name), GL_UNIFORM),
ABlockResources(),
_value(value)
{

}

Mat4::Mat4(Mat4 &&move):
AProgramResources(std::move(move)),
ABlockResources(move),
_value(move._value)
{

}

Mat4 &Mat4::operator=(glm::mat4 const &m)
{
	_value = m;
	return (*this);
}

IProgramResources & Mat4::operator()()
{
	if (!_update) {
		glUniformMatrix4fv(_id, 1, GL_FALSE, (GLfloat *)(&_value[0]));
		_update = true;
	}
	return (*this);
}

bool Mat4::safe(size_t size) const
{
	return ((sizeof(type) == size) ? true : false);
}

size_t Mat4::size() const
{
	return (sizeof(glm::mat4));
}

void const *Mat4::data() const
{
	return (&_value);
}
