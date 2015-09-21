#include <Render/Textures/ATexture.hh>

namespace AGE
{
	ATexture::ATexture() :
		_id(-1),
		_width(0),
		_height(0),
		_internal_format(0),
		_nbr_mip_map(0)
	{

	}

	bool ATexture::init(GLint width, GLint height, GLenum internal_format, GLint nbr_mip_map)
	{
		_width = width;
		_height = height;
		_internal_format = internal_format;
		_nbr_mip_map = nbr_mip_map;
		glGenTextures(1, &_id);
		if (_id == GLuint(- 1))
		{
			return false;
		}
		return true;
	}

	ATexture::ATexture(ATexture &&move) :
		_id(move._id),
		_width(move._width),
		_height(move._height),
		_internal_format(move._internal_format),
		_nbr_mip_map(move._nbr_mip_map)
	{
		move._id = 0;
	}

	ATexture::~ATexture()
	{
		if (_id == -1) {
			glDeleteTextures(1, &_id);
		}
	}


	GLuint ATexture::id() const
	{
		return (_id);
	}

	GLint ATexture::width() const
	{
		return (_width);
	}

	GLint ATexture::height() const
	{
		return (_height);
	}

	GLenum ATexture::internal_format() const
	{
		return (_internal_format);
	}
}