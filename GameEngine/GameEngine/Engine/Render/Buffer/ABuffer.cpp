#include <Render/Buffer/ABuffer.hh>

ABuffer::ABuffer() :
_size(0)
, _binded(false)
{
	glGenBuffers(1, &_id);
}

ABuffer::ABuffer(ABuffer &&move) :
_id(move._id),
_size(move._size),
_binded(move._binded)
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

void ABuffer::bind()
{
	if (_binded)
	{
		return;
	}
	_binded = true;
	_bind();
}

void ABuffer::unbind()
{
	if (!_binded)
	{
		return;
	}
	_binded = false;
	_unbind();
}

bool ABuffer::isBinded() const
{
	return _binded;
}