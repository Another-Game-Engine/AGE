#include <Render/Buffer/VertexBuffer.hh>

VertexBuffer::VertexBuffer() :
ABuffer()
{

}

IBuffer const & VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, _id);
	return (*this);
}

IBuffer const & VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (*this);
}

GLenum VertexBuffer::mode() const
{
	return (GL_ARRAY_BUFFER);
}

IBuffer const & VertexBuffer::alloc(size_t size)
{
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	_size = size;
	return (*this);
}

IBuffer const & VertexBuffer::sub(size_t offset, size_t size, void const *buffer) const
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, buffer);
	return (*this);
}