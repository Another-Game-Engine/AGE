#include <Render/Buffer/VertexArray.hh>

GLuint VertexArray::_current_id = -1;

VertexArray::VertexArray() :
_id(0)
{
	glGenVertexArrays(1, &_id);
}

VertexArray::~VertexArray()
{
	if (_id) {
		glDeleteVertexArrays(1, &_id);
	}
}

VertexArray::VertexArray(VertexArray &&move) :
_id(move._id)
{
	move._id = 0;
}

void VertexArray::bind() const
{
	if (_current_id != _id) {
		glBindVertexArray(_id);
		_current_id = _id;
	}
}

void VertexArray::unbind() const
{
	_current_id = 0;
	glBindVertexArray(0);
}

GLuint VertexArray::getId() const
{
	return (_id);
}