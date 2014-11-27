#include <Render/Buffer/ABuffer.hh>

ABuffer::ABuffer()
{
	glGenBuffers(1, &_id);
}

ABuffer::ABuffer(ABuffer &&move) :
_id(move._id)
{
	move._id = 0;
}

ABuffer::~ABuffer()
{
	if (_id) {
		glDeleteBuffers(1, &_id);
	}
}

ABuffer & ABuffer::operator=(ABuffer &&i)
{
	_id = i._id;
	i._id = 0;
	return (*this);
}

/**
* Method:    id
* FullName:  ABuffer::id
* Access:    public 
* Returns:   GLuint
* Qualifier: const
* Goal:		 get the openGL id of the buffer
*/
GLuint ABuffer::id() const
{
	return (_id);
}

