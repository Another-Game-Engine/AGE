#include <Render/Buffer/IndexBuffer.hh>

IndexBuffer *IndexBuffer::_lastBinded = nullptr;

void IndexBuffer::_bind()
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
}


void IndexBuffer::_unbind()
{
	if (_lastBinded != this)
	{
		return;
	}
	_lastBinded = nullptr;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLenum IndexBuffer::mode() const
{
	return (GL_ELEMENT_ARRAY_BUFFER);
}

ABuffer const & IndexBuffer::alloc(size_t size)
{
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	_size = size;
	return (*this);
}

ABuffer const & IndexBuffer::sub(size_t offset, size_t size, void const *buffer) const
{
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, buffer);
	return (*this);
}

