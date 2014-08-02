#include <Render/Texture.hh>
#include <utility>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <string>
#include <glm/glm.hpp>

namespace gl
{
	Storage::Storage(GLsizei width, GLsizei height, GLenum internalFormat)
		: _id(0),
		_width(width),
		_height(height),
		_internalFormat(internalFormat)
	{
	}

	Storage::~Storage()
	{
	}


	Texture::Texture(GLsizei width, GLsizei height, GLenum internalFormat, bool mipMapping)
		: Storage(width, height, internalFormat),
		_mipMapLevels(mipMapping ? (uint8_t(glm::floor(glm::log2(glm::max(float(_width), float(_height))) + 1))) : 1)
	{
		glGenTextures(1, &_id);
	}

	Texture::~Texture()
	{
		glDeleteTextures(1, &_id);
	}

	uint8_t Texture::getMaxLevelMipMap() const
	{
		return (_mipMapLevels);
	}

	Texture2D::Texture2D(GLsizei width, GLsizei height, GLenum internalFormat, bool mipMapping)
		: Texture(width, height, internalFormat, mipMapping),
		_rect(glm::ivec4(0, 0, width, height))
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexStorage2D(GL_TEXTURE_2D, getMaxLevelMipMap(), _internalFormat, _width, _height);
	}

	Texture2D::~Texture2D()
	{
	}

	Texture const &Texture2D::generateMipMap() const
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		return (*this);
	}

	GLenum Texture2D::getType() const
	{
		return (GL_TEXTURE_2D);
	}

	GLuint Texture2D::getId() const
	{
		return (_id);
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

	Texture const &Texture2D::download(GLint level, GLenum format, GLenum type, GLvoid *img) const
	{
		glGetTexImage(GL_TEXTURE_2D, level, format, type, img);
		return (*this);
	}

	Texture2D &Texture2D::configUpload(glm::ivec4 const &rect)
	{
		_rect = rect;
		return (*this);
	}

	Texture const &Texture2D::upload(GLint level, GLenum format, GLenum type, GLvoid *img) const
	{
		glTexSubImage2D(GL_TEXTURE_2D, level, _rect.x, _rect.y, _rect.z, _rect.w, format, type, img);
		return (*this);
	}

	Texture const &Texture2D::parameter(GLenum pname, GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, pname, param);
		return (*this);
	}


}