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
	}

	Texture::~Texture()
	{
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
		_format(GL_BGRA),
		_type(GL_UNSIGNED_BYTE)
	{
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

	
	Texture &Texture2D::init()
	{
		glGenTextures(1, &_id);
		glBindTexture(GL_TEXTURE_2D, _id);
		std::cout << _levels << std::endl;
		std::cout << _width << std::endl;
		glTexStorage2D(GL_TEXTURE_2D, _levels, _format, _width, _height);
		GLenum mode = glGetError();
		std::string error = (mode == GL_INVALID_VALUE) ? "storage invalid value" : "";
		std::cout << error << std::endl;
		return (*this);
	}

	void Texture2D::unload() const
	{
		glDeleteTextures(1, &_id);
	}

	Texture2D const &Texture2D::wrap(GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
		return (*this);
	}

	Texture2D const &Texture2D::filter(GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		return (*this);
	}

	Texture2D const &Texture2D::filterMag(GLenum param) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		return (*this);
	}

	Texture2D const &Texture2D::filterMin(GLenum param) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		return (*this);
	}

	Texture2D const &Texture2D::storage(GLint param) const
	{
		glPixelStorei(GL_PACK_ALIGNMENT, param);
		glPixelStorei(GL_UNPACK_ALIGNMENT, param);
		return (*this);
	}

	Texture2D const &Texture2D::storagePack(GLint param) const
	{
		glPixelStorei(GL_PACK_ALIGNMENT, param);
		return (*this);
	}

	Texture2D const &Texture2D::storageUnPack(GLint param) const
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, param);
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

	Texture2D &Texture2D::setOptionReadWrite(GLint level, GLenum format, GLenum type)
	{
		_level = level;
		_format = format;
		_type = type;
		return (*this);
	}

	Texture &Texture2D::startRead(void **read)
	{
		_out.bind();
		glGetTexImage(GL_TEXTURE_2D, _level, _format, _type, 0);
		_out.mapBuffer(read);
		return (*this);
	}

	Texture &Texture2D::stopRead(void **read)
	{
		_out.unmapBuffer(read).unbind();
		return (*this);
	}

	Texture &Texture2D::write(void *write)
	{
		std::uint32_t sizeWrite = _height * _width;

		_in.bind();
		glTexSubImage2D(GL_TEXTURE_2D, _level, 0, 0, _width, _height, _format, _type, 0);
		_in.setBuffer(write, sizeWrite).unbind();
		return (*this);
	}

	Texture &Texture2D::writeFlush(void *write)
	{
		std::cout << "level = " << _level << std::endl;
		std::cout << "log max = " << std::log2(GL_MAX_TEXTURE_SIZE) << std::endl;
		std::cout << "width = " << _width << std::endl;
		std::cout << "height = " << _height << std::endl;
		glTexSubImage2D(GL_TEXTURE_2D, _level, 0, 0, _width, _height, _format, _type, write);
		GLenum mode = glGetError();
		std::string error = (mode == GL_INVALID_VALUE) ? "write invalid value" : "";
		std::cout << error << std::endl;
		return (*this);
	}

	Texture2D const &Texture2D::generateMipMap() const
	{
		if (_levels > 1)
			glGenerateMipmap(GL_TEXTURE_2D);
		GLenum mode = glGetError();
		std::string error = (mode == GL_INVALID_VALUE) ? "generate invalid value" : "";
		std::cout << error << std::endl;
		return (*this);
	}

}