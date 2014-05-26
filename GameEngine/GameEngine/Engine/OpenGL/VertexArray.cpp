#include "VertexArray.hh"

namespace gl
{
	VertexArray::VertexArray()
		: _id(0),
		_isBind(false)
	{
	}

	void VertexArray::init()
	{
		glGenVertexArrays(1, &_id);
	}

	VertexArray::~VertexArray()
	{
		if (_id)
			glDeleteVertexArrays(1, &_id);
	}

	VertexArray::VertexArray(VertexArray const &copy)
		: _id(copy._id),
		_isBind(copy._isBind)
	{
	}

	VertexArray &VertexArray::operator=(VertexArray const &vertexarray)
	{
		_id = vertexarray._id;
		_isBind = vertexarray._isBind;
		return (*this);
	}

	void VertexArray::bind()
	{
		glBindVertexArray(_id);
	}

	void VertexArray::unbind()
	{
		glBindVertexArray(0);
	}

	GLuint VertexArray::getId() const
	{
		return (_id);
	}

}