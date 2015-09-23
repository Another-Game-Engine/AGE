#include <Render/Buffer/ABuffer.hh>

ABuffer::ABuffer() :
_size(0)
{
	glGenBuffers(1, &_id);
}

ABuffer::ABuffer(ABuffer &&move) :
_id(move._id),
_size(move._size)
{
	move._id = 0;
}

ABuffer::~ABuffer()
{
	if (_id) {
		glDeleteBuffers(1, &_id);
	}
}

GLuint ABuffer::id() const
{
	return (_id);
}

size_t ABuffer::size() const
{
	return (_size);
}
