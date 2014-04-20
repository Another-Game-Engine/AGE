#include <OpenGL/Texture.hh>
#include <utility>

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
		: _id(copy._id)
	{
	}

	Texture::Texture(Texture &&move)
		: _id(std::move(move._id))
	{
	}

	Texture &Texture::operator=(Texture const &t)
	{
		_id = t._id;
		return (*this);
	}

	Texture &Texture::operator=(Texture &&t)
	{
		_id = std::move(t._id);
		return (*this);
	}

	GLuint Texture::getId() const
	{
		return (_id);
	}

	Texture2D::Texture2D(GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height)
		: Texture(),
		_levels(levels),
		_width(width),
		_height(height),
		_internalFormat(internalFormat)
	{
		glGenBuffers(1, &_id);
		glBindBuffer(GL_TEXTURE_2D, _id);
		glTexStorage2D(GL_TEXTURE_2D, levels, internalFormat, width, height);
	}

	Texture2D::~Texture2D()
	{
	}

	Texture2D::Texture2D(Texture2D const &copy)
		:Texture(copy),
		_levels(copy._levels),
		_width(copy._width),
		_height(copy._height),
		_internalFormat(copy._internalFormat)
	{
	}

	Texture2D::Texture2D(Texture2D &&move)
		: Texture(std::move(move)),
		_levels(std::move(move._levels)),
		_width(std::move(move._width)),
		_height(std::move(move._height)),
		_internalFormat(std::move(move._internalFormat))
	{
	}

	Texture2D &Texture2D::operator=(Texture2D const &t)
	{
		_id = t._id;
		_levels = t._levels;
		_width = t._width;
		_height = t._height;
		_internalFormat = t._internalFormat;
		return (*this);
	}

	Texture2D &Texture2D::operator=(Texture2D &&t)
	{
		_id = std::move(t._id);
		_levels = std::move(t._levels);
		_width = std::move(t._width);
		_height = std::move(t._height);
		_internalFormat = std::move(t._internalFormat);
		return (*this);
	}

	inline void Texture2D::wrap(GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	}

	inline void Texture2D::filter(GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
	}

	inline void Texture2D::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, _id);
	}

	inline void Texture2D::unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}