#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/Program.hh>

Mat4::Mat4(glm::mat4 const &value, GLint id, std::string &&name) :
AProgramResources(id, std::move(name), GL_UNIFORM),
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

Mat4::Mat4(Mat4 const &copy) :
AProgramResources(copy),
ABlockResources(copy),
_value(copy._value)
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

void Mat4::print() const
{
	std::cout << "uniform mat4 " << _name << ";";
	std::cout << std::endl;
}
