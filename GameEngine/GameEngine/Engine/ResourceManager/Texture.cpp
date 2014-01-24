#include "Texture.hh"

#include "Parsers/Parsers.hh"
#include "Utils/OpenGL.hh"

namespace	Resources
{

	Texture::Texture(void) :
		_id(0),
		_wrap(GL_REPEAT),
		_minFilter(GL_LINEAR_MIPMAP_LINEAR),
		_magFilter(GL_LINEAR)
	{
	}


	Texture::~Texture(void)
	{
		glDeleteTextures(1, &_id);
	}

	bool			Texture::load(std::string const &path)
	{
		GLbyte		*datas;
		GLint		width, height;
		GLint		components;
		GLenum		format;
		unsigned int size;
		if ((datas = loadTGA(path.c_str(), &width, &height, &components, &format, &size)) == NULL)
			return (false);

		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _wrap);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _magFilter);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, components, width, height, 0, format, GL_UNSIGNED_BYTE, datas);
		delete[] datas;
		if (_minFilter == GL_LINEAR_MIPMAP_LINEAR ||
			_minFilter == GL_LINEAR_MIPMAP_NEAREST ||
			_minFilter == GL_NEAREST_MIPMAP_LINEAR ||
			_minFilter == GL_NEAREST_MIPMAP_NEAREST)
			glGenerateMipmap(GL_TEXTURE_2D);
		return (true);
	}

	const GLuint			Texture::getId() const
	{
		return (_id);
	}
}
