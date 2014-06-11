#include "Buffer.hh"

namespace gl
{
	Buffer::Buffer()
	{
		glGenBuffers(1, &_id);
	}

	Buffer::~Buffer()
	{
		if (_id)
			glDeleteBuffers(1, &_id);
	}

	GLuint Buffer::getId() const
	{
		return (_id);
	}

	VertexBuffer::VertexBuffer()
		: Buffer()
	{
	}

	VertexBuffer::~VertexBuffer()
	{
	}

	Buffer const &VertexBuffer::bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, _id);
		return (*this);
	}

	Buffer const &VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		return (*this);
	}

	GLenum VertexBuffer::getMode() const
	{
		return (GL_ARRAY_BUFFER);
	}

	IndexBuffer::IndexBuffer()
		: Buffer()
	{
	}

	IndexBuffer::~IndexBuffer()
	{
	}

	Buffer const &IndexBuffer::bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
		return (*this);
	}

	Buffer const &IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		return (*this);
	}

	GLenum IndexBuffer::getMode() const
	{
		return (GL_ELEMENT_ARRAY_BUFFER);
	}
}