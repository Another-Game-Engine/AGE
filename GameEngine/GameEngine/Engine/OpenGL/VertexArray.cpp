#include "VertexArray.hh"
# include "ErrorController.hh"

namespace OpenGLTools
{
	VertexArray::VertexArray()
		: _id(0),
		_isBind(false)
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
		if (_isBind == false)
		{
			_isBind = true;
			glBindVertexArray(_id);
		}
	}

	void VertexArray::unbind()
	{
		_isBind = false;
		glBindVertexArray(0);
	}

	GLuint VertexArray::getId() const
	{
		return (_id);
	}

}