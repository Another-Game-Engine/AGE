#include <Render/Buffer/UniformBuffer.hh>

/**
* Method:    bind
* FullName:  UniformBuffer::bind
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Goal:		 bind the uniform buffer
*/
IBuffer const & UniformBuffer::bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, _id);
	return (*this);
}

/**
* Method:    unbind
* FullName:  UniformBuffer::unbind
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Goal:		 unbind the uniform buffer
*/
IBuffer const & UniformBuffer::unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return (*this);
}

/**
* Method:    mode
* FullName:  UniformBuffer::mode
* Access:    virtual public 
* Returns:   GLenum
* Qualifier: const
* Goal:		 get the openGL mode
*/
GLenum UniformBuffer::mode() const
{
	return (GL_UNIFORM_BUFFER);
}

/**
* Method:    alloc
* FullName:  UniformBuffer::alloc
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Parameter: size_t size
* Goal:		 alloc the buffer of "size"
*/
IBuffer const & UniformBuffer::alloc(size_t size) const
{
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	return (*this);
}

/**
* Method:    sub
* FullName:  UniformBuffer::sub
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Parameter: size_t offset
* Parameter: size_t size
* Parameter: void const * buffer
* Goal:		 set a sub set of data the buffer
*/
IBuffer const & UniformBuffer::sub(size_t offset, size_t size, void const *buffer) const
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, buffer);
	return (*this);
}

