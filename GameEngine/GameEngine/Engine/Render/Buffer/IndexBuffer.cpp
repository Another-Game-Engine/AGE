#include <Render/Buffer/IndexBuffer.hh>

/**
* Method:    bind
* FullName:  IndexBuffer::bind
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Goal:		 bind the index buffer
*/
IBuffer const & IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	return (*this);
}

/**
* Method:    unbind
* FullName:  IndexBuffer::unbind
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Goal:		 unbind the index buffer
*/
IBuffer const & IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return (*this);
}

/**
* Method:    mode
* FullName:  IndexBuffer::mode
* Access:    virtual public 
* Returns:   GLenum
* Qualifier: const
* Goal:		 get the openGL mode
*/
GLenum IndexBuffer::mode() const
{
	return (GL_ELEMENT_ARRAY_BUFFER);
}

/**
* Method:    alloc
* FullName:  IndexBuffer::alloc
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Parameter: size_t size
* Goal:		 alloc the buffer with "size" value
*/
IBuffer const & IndexBuffer::alloc(size_t size) const
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	return (*this);
}

/**
* Method:    sub
* FullName:  IndexBuffer::sub
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Parameter: size_t offset
* Parameter: size_t size
* Parameter: void const * buffer
* Goal:		 set a sub set of data into the buffer
*/
IBuffer const & IndexBuffer::sub(size_t offset, size_t size, void const *buffer) const
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, buffer);
	return (*this);
}

