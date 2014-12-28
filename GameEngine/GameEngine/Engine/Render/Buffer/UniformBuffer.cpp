#include <Render/Buffer/UniformBuffer.hh>

IBuffer const & UniformBuffer::bind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, _id);
	return (*this);
}

IBuffer const & UniformBuffer::unbind() const
{
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return (*this);
}

GLenum UniformBuffer::mode() const
{
	return (GL_UNIFORM_BUFFER);
}

IBuffer const & UniformBuffer::alloc(size_t size) const
{
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	return (*this);
}

IBuffer const & UniformBuffer::sub(size_t offset, size_t size, void const *buffer) const
{
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, buffer);
	return (*this);
}

