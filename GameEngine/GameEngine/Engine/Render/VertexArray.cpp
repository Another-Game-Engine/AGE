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

}