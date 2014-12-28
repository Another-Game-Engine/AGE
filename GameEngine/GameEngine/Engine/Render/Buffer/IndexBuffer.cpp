#include <Render/Buffer/IndexBuffer.hh>

IBuffer const & IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
	return (*this);
}


IBuffer const & IndexBuffer::unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	return (*this);
}

GLenum IndexBuffer::mode() const
{
	return (GL_ELEMENT_ARRAY_BUFFER);
}

IBuffer const & IndexBuffer::alloc(size_t size) const
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	return (*this);
}

IBuffer const & IndexBuffer::sub(size_t offset, size_t size, void const *buffer) const
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, buffer);
	return (*this);
}

