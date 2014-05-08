#include <OpenGL/Texture.hh>
#include <utility>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <string>


namespace OpenGLTools
{
	Texture::Texture()
		: _id(0)
	{
		glGenTextures(1, &_id);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_id);
	}

	Texture::Texture(Texture const &copy)
		: _id(copy._id),
		_out(copy._out),
		_in(copy._in)
	{
	}

	Texture::Texture(Texture &&move)
		: _id(std::move(move._id)),
		_out(std::move(move._out)),
		_in(std::move(move._in))
	{
	}

	Texture &Texture::operator=(Texture const &t)
	{
		_id = t._id;
		_out = t._out;
		_in = t._in;
		return (*this);
	}

	Texture &Texture::operator=(Texture &&t)
	{
		_id = std::move(t._id);
		_out = std::move(t._out);
		_in = std::move(t._in);
		return (*this);
	}

	GLuint Texture::getId() const
	{
		return (_id);
	}

	Texture2D::Texture2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
		: Texture(),
		_levels(levels),
		_level(0),
		_width(width),
		_height(height),
		_internalFormat(internalFormat),
		_format(GL_RGBA),
		_type(GL_UNSIGNED_BYTE)
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		_levels = _levels == 0 ? 1 : _levels;
		glTexStorage2D(GL_TEXTURE_2D, _levels, _internalFormat, _width, _height);
	}

	Texture2D::~Texture2D()
	{
	}

	Texture2D::Texture2D(Texture2D const &copy)
		:Texture(copy),
		_levels(copy._levels),
		_level(copy._level),
		_width(copy._width),
		_height(copy._height),
		_internalFormat(copy._internalFormat),
		_format(copy._format),
		_type(copy._type)
	{
	}

	Texture2D::Texture2D(Texture2D &&move)
		: Texture(std::move(move)),
		_levels(std::move(move._levels)),
		_level(std::move(move._level)),
		_width(std::move(move._width)),
		_height(std::move(move._height)),
		_internalFormat(std::move(move._internalFormat)),
		_format(std::move(move._format)),
		_type(std::move(move._type))
	{
	}

	Texture2D &Texture2D::operator=(Texture2D const &t)
	{
		_id = t._id;
		_levels = t._levels;
		_level = t._level;
		_width = t._width;
		_height = t._height;
		_internalFormat = t._internalFormat;
		_format = t._format;
		_type = t._type;
		return (*this);
	}

	Texture2D &Texture2D::operator=(Texture2D &&t)
	{
		_id = std::move(t._id);
		_levels = std::move(t._levels);
		_level = std::move(t._level);
		_width = std::move(t._width);
		_height = std::move(t._height);
		_internalFormat = std::move(t._internalFormat);
		_internalFormat = std::move(t._internalFormat);
		return (*this);
	}

	Texture2D const &Texture2D::wrap(GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
		return (*this);
	}

	Texture2D const &Texture2D::filter(GLint minFilter, GLint magFilter) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
		return (*this);
	}

	Texture2D const &Texture2D::filter(GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		return (*this);
	}

	Texture2D const &Texture2D::storage(GLint param) const
	{
		glPixelStorei(GL_PACK_ALIGNMENT, param);
		glPixelStorei(GL_UNPACK_ALIGNMENT, param);
		return (*this);
	}

	Texture2D const &Texture2D::storage(GLint pack, GLint unpack) const
	{
		glPixelStorei(GL_PACK_ALIGNMENT, pack);
		glPixelStorei(GL_UNPACK_ALIGNMENT, unpack);
		return (*this);
	}

	Texture const &Texture2D::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		return (*this);
	}

	Texture const &Texture2D::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		return (*this);
	}

	Texture2D &Texture2D::setOptionTransfer(GLint level, GLenum format, GLenum type)
	{
		_level = level;
		_format = format;
		_type = type;
		return (*this);
	}

	void *Texture2D::read(void *read) const
	{
		glGetTexImage(GL_TEXTURE_2D, _level, _format, _type, read);
		return (read);
	}

	void Texture2D::write(void *write) const
	{
		glTexSubImage2D(GL_TEXTURE_2D, _level, 0, 0, _width, _height, _format, _type, write);
	}

	Texture2D const &Texture2D::generateMipMap() const
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		return (*this);
	}

	GLenum Texture2D::getType() const
	{
		return (GL_TEXTURE_2D);
	}

	TextureMultiSample::TextureMultiSample(GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocation)
		: Texture(),
		_samples(samples),
		_width(width),
		_height(height),
		_internalFormat(internalFormat),
		_format(GL_BGRA),
		_type(GL_UNSIGNED_BYTE),
		_fixedSampleLocation(fixedSampleLocation)
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _id);
		glTexStorage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _samples, _internalFormat, _width, _height, _fixedSampleLocation);
	}

	TextureMultiSample::~TextureMultiSample()
	{
	}

	Texture const &TextureMultiSample::bind() const
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, _id);
		return (*this);
	}

	Texture const &TextureMultiSample::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
		return (*this);
	}

	void TextureMultiSample::write(void *) const
	{

	}

	void *TextureMultiSample::read(void *read) const
	{
		return (NULL);
	}

	GLenum TextureMultiSample::getType() const
	{
		return (GL_TEXTURE_2D_MULTISAMPLE);
	}
}