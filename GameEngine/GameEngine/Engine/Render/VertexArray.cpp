#include <Render/VertexArray.hh>

namespace gl
{
	VertexArray::VertexArray()
		: _id(0)
	{
		glGenVertexArrays(1, &_id);
	}

	VertexArray::~VertexArray()
	{
		if (_id)
			glDeleteVertexArrays(1, &_id);
	}

	void VertexArray::bind() const
	{
		glBindVertexArray(_id);
	}

	void VertexArray::unbind() const
	{
		glBindVertexArray(0);
	}

	GLuint VertexArray::getId() const
	{
		return (_id);
	}

	VertexArray const &VertexArray::activateAttribute(size_t index) const
	{
		glEnableVertexAttribArray(GLuint(index));
		return (*this);
	}

	VertexArray const &VertexArray::attribute(size_t index, int nbrComponent, GLenum type, size_t offset) const
	{
		glVertexAttribPointer(GLuint(index), GLint(nbrComponent), type, GL_FALSE, 0, (const GLvoid *)offset);
		return (*this);
	}
}