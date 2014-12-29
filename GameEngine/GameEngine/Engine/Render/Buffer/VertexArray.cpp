#include <Render/Buffer/VertexArray.hh>

VertexArray::VertexArray() :
_id(0)
{
	glGenVertexArrays(1, &_id);
}

VertexArray::~VertexArray()
{
	if (_id)
		glDeleteVertexArrays(1, &_id);
}

VertexArray::VertexArray(VertexArray &&move) :
_id(move._id)
{
	_id = 0;
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