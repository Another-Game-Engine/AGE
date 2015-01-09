#include <Render/Storage/AStorage.hh>
# include <Render/Buffer/Framebuffer.hh>

AStorage::AStorage(GLint weight, GLint height, GLenum internal_format) :
_id(0),
_weight(weight),
_height(height),
_internal_format(internal_format)
{
	glGenTextures(1, &_id);
}

AStorage::AStorage(AStorage &&move) :
_id(move._id),
_weight(move._weight),
_height(move._height),
_internal_format(move._internal_format)
{
	move._id = 0;
}

AStorage::~AStorage()
{
	if (_id) {
		glDeleteTextures(1, &_id);
	}
}


GLuint AStorage::id() const
{
	return (_id);
}

GLint AStorage::weight() const
{
	return (_weight);
}

GLint AStorage::height() const
{
	return (_height);
}

GLenum AStorage::internal_format() const
{
	return (_internal_format);
}