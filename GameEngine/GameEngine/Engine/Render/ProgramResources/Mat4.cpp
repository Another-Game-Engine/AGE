#include <Render/ProgramResources/Mat4.hh>
#include <Render/Program.hh>

Mat4::Mat4(glm::mat4 const &value, Program const &parent, std::string const &name) :
AProgramResources(parent, name, GL_UNIFORM),
ABlockResources(),
_value(value)
{

}

Mat4::Mat4(glm::mat4 const &value, GLint id, std::string &&name) :
AProgramResources(id, std::move(name), GL_UNIFORM),
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

/**
* Method:    operator()
* FullName:  Mat4::operator()
* Access:    virtual public 
* Returns:   IProgramResource &
* Qualifier:
* Goal:		 send the data to GPU
*/
IProgramResources & Mat4::operator()()
{
	if (!_update) {
		glUniformMatrix4fv(_id, 1, GL_FALSE, (GLfloat *)(&_value[0]));
		_update = true;
	}
	return (*this);
}

/**
* Method:    safe
* FullName:  Mat4::safe
* Access:    virtual public 
* Returns:   bool
* Qualifier: const
* Parameter: size_t size
* Goal:		 Valid if the memory size in input is the same than in intern
*/
bool Mat4::safe(size_t size) const
{
	return ((sizeof(type) == size) ? true : false);
}

/**
* Method:    size
* FullName:  Mat4::size
* Access:    virtual public 
* Returns:   size_t
* Qualifier: const
* Goal:		 size of element
*/
size_t Mat4::size() const
{
	return (sizeof(glm::mat4));
}

/**
* Method:    data
* FullName:  Mat4::data
* Access:    virtual public 
* Returns:   void const *
* Qualifier: const
* Goal:		 data the mat4
*/
void const *Mat4::data() const
{
	return (&_value);
}
