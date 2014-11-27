#include <Render/Buffer/VertexBuffer.hh>

/**
* Method:    bind
* FullName:  VertexBuffer::bind
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Goal:		 bind the vertex buffer
*/
IBuffer const & VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	return (*this);
}

/**
* Method:    unbind
* FullName:  VertexBuffer::unbind
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Goal:		 unbind the vertex buffer
*/
IBuffer const & VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (*this);
}

/**
* Method:    mode
* FullName:  VertexBuffer::mode
* Access:    virtual public 
* Returns:   GLenum
* Qualifier: const
* Goal:		 get the openGL mode of the buffer
*/
GLenum VertexBuffer::mode() const
{
	return (GL_ARRAY_BUFFER);
}


/**
* Method:    alloc
* FullName:  VertexBuffer::alloc
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Parameter: size_t size
* Goal:		 
*/
IBuffer const & VertexBuffer::alloc(size_t size) const
{
	glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
	return (*this);
}

/**
* Method:    sub
* FullName:  VertexBuffer::sub
* Access:    virtual public 
* Returns:   IBuffer const &
* Qualifier: const
* Parameter: size_t offset
* Parameter: size_t size
* Parameter: void const * buffer
* Goal:		 set a sub sets of into the buffer
*/
IBuffer const & VertexBuffer::sub(size_t offset, size_t size, void const *buffer) const
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, buffer);
	return (*this);
}