#include <Render/ProgramResources/Mat4.hh>
#include <Render/Program.hh>

Mat4::Mat4(glm::mat4 const &value, Program const &parent, std::string const &name) :
AProgramResources(parent, name, GL_UNIFORM)
{

}

Mat4::Mat4(glm::mat4 const &value, Program const &parent, std::string &&name):
AProgramResources(parent, name, GL_UNIFORM),
_value(value)
{

}

Mat4::Mat4(Mat4 &&move):
AProgramResources(std::move(move)),
_value(move._value)
{

}

Mat4 & Mat4::operator=(glm::mat4 const &value)
{
	_value = value;
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
IProgramResource & Mat4::operator()()
{
	if (!_isUpdate) {
		glUniformMatrix4fv(_id, 1, GL_FALSE, (GLfloat *)(&_value[0]));
		_isUpdate = true;
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
