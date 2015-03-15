#include <Render/Buffer/VertexBuffer.hh>

VertexBuffer *VertexBuffer::_lastBinded = nullptr;

VertexBuffer::VertexBuffer() :
	ABuffer()
{

}

void VertexBuffer::_bind()
{
	if (_lastBinded == this)
	{
		return;
	}
	if (_lastBinded)
	{
		_lastBinded->unbind();
	}

	_lastBinded = this;
	glBindBuffer(GL_ARRAY_BUFFER, _id);
}

void VertexBuffer::_unbind()
{
	if (_lastBinded != this)
	{
		return;
	}
	_lastBinded = nullptr;
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLenum VertexBuffer::mode() const
{
	return (GL_ARRAY_BUFFER);
}

ABuffer const & VertexBuffer::alloc(size_t size)
{
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	_size = size;
	return (*this);
}

ABuffer const & VertexBuffer::sub(size_t offset, size_t size, void const *buffer) const
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, buffer);
	return (*this);
}