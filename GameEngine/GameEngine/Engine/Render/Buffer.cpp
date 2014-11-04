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

	Buffer const &VertexBuffer::BufferData(size_t size) const
	{
		glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		return (*this);
	}

	VertexBuffer const &VertexBuffer::attribute(size_t index, int nbrComponent, GLenum type, size_t offset) const
	{
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(GLuint(index), GLint(nbrComponent), type, GL_FALSE, 0, (const GLvoid *)offset);
		return (*this);
	}
	
	Buffer const &VertexBuffer::BufferSubData(size_t offset, size_t size, void *buffer) const
	{
		glBufferSubData(GL_ARRAY_BUFFER, offset, size, buffer);
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

	Buffer const &IndexBuffer::BufferData(size_t size) const
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
		return (*this);
	}

	Buffer const &IndexBuffer::BufferSubData(size_t offset, size_t size, void *buffer) const
	{
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, buffer);
		return (*this);
	}

	UniformBuffer::UniformBuffer()
		: Buffer()
	{
	}

	UniformBuffer::~UniformBuffer()
	{
	}

	Buffer const &UniformBuffer::bind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, _id);
		return (*this);
	}

	Buffer const &UniformBuffer::unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		return (*this);
	}

	GLenum UniformBuffer::getMode() const
	{
		return (GL_UNIFORM_BUFFER);
	}

	Buffer const &UniformBuffer::BufferData(size_t size) const
	{
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		return (*this);
	}

	Buffer const &UniformBuffer::BufferSubData(size_t offset, size_t size, void *buffer) const
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, buffer);
		return (*this);
	}
}