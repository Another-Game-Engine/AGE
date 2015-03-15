#include <Render/Buffer/VertexArray.hh>

VertexArray *VertexArray::_lastBinded = nullptr;

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

void VertexArray::bind()
{
	if (_lastBinded == this)
	{
		return;
	}
	_lastBinded = this;
	glBindVertexArray(_id);

}

void VertexArray::unbind()
{
	if (_lastBinded != this)
	{
		return;
	}
	_lastBinded = nullptr;
}

static void Unbind()
{
	glBindVertexArray(0);
}

GLuint VertexArray::getId() const
{
	return (_id);
}