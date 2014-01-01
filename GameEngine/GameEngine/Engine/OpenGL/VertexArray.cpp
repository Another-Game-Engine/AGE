#include "VertexArray.hh"
# include "ErrorController.hh"

namespace OpenGLTools
{
	VertexArray::VertexArray()
		: _id(0)
	{
	}

	VertexArray::~VertexArray()
	{
		if (_id)
			glDeleteVertexArrays(1, &_id);
	}

	void VertexArray::init()
	{
		glGenVertexArrays(1, &_id);
	}

	void VertexArray::unload()
	{
		if (_id)
		{
			glDeleteVertexArrays(1, &_id);
			_id = 0;
		}
	}

	VertexArray::VertexArray(VertexArray const &copy)
		: _id(copy._id)
	{
	}

	VertexArray &VertexArray::operator=(VertexArray const &vertexarray)
	{
		_id = vertexarray._id;
		return (*this);
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