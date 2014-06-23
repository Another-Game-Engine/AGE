#include "Buffer.hh"

namespace gl
{
	GLuint VertexBuffer::_currentId = 0;
	GLuint IndexBuffer::_currentId = 0;
	GLuint UniformBuffer::_currentId = 0;

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
		if (_currentId != _id)
		{
			glBindBuffer(GL_ARRAY_BUFFER, _id);
			_currentId = _id;
		}
		return (*this);
	}

	Buffer const &VertexBuffer::unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		_currentId = 0;
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
		if (_currentId != _id)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _id);
			_currentId = _id;
		}
		return (*this);
	}

	Buffer const &IndexBuffer::unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		_currentId = 0;
		return (*this);
	}

	GLenum IndexBuffer::getMode() const
	{
		return (GL_ELEMENT_ARRAY_BUFFER);
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
		if (_currentId != _id)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, _id);
			_currentId = _id;
		}
		return (*this);
	}

	Buffer const &UniformBuffer::unbind() const
	{
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		_currentId = 0;
		return (*this);
	}

	GLenum UniformBuffer::getMode() const
	{
		return (GL_UNIFORM_BUFFER);
	}
}