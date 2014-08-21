#include <Render/Storage.hh>
#include <Render/Framebuffer.hh>
#include <utility>
#include <cstdint>
#include <iostream>
#include <algorithm>
#include <string>
#include <glm/glm.hpp>

namespace gl
{
	Storage::Storage(size_t width, size_t height, GLenum internalFormat)
		: _id(0),
		_width(width),
		_height(height),
		_internalFormat(internalFormat)
	{
	}

	Storage::~Storage()
	{
	}

	GLuint Storage::getId() const
	{
		return (_id);
	}

	Texture::Texture(size_t width, size_t height, GLenum internalFormat, bool mipMapping)
		: Storage(width, height, internalFormat),
		_mipMapLevels(mipMapping ? (uint8_t(glm::floor(glm::log2(glm::max(float(_width), float(_height))) + 1))) : 1),
		_levelTarget(0)
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

	Texture &Texture::setLevelTarget(uint8_t target)
	{
		_levelTarget = target;
		return (*this);
	}

	Texture2D::Texture2D(size_t width, size_t height, GLenum internalFormat, bool mipMapping)
		: Texture(width, height, internalFormat, mipMapping),
		_rect(glm::ivec4(0, 0, width, height))
	{
		glBindTexture(GL_TEXTURE_2D, _id);
		glTexStorage2D(GL_TEXTURE_2D, _mipMapLevels, _internalFormat, GLsizei(_width), GLsizei(_height));
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

	Texture const &Texture2D::download(GLenum format, GLenum type, GLvoid *img) const
	{
		glGetTexImage(GL_TEXTURE_2D, _levelTarget, format, type, img);
		return (*this);
	}

	Texture2D &Texture2D::configUpload(glm::ivec4 const &rect)
	{
		_rect = rect;
		return (*this);
	}

	Texture const &Texture2D::upload(GLenum format, GLenum type, GLvoid *img) const
	{
		glTexSubImage2D(GL_TEXTURE_2D, _levelTarget, _rect.x, _rect.y, _rect.z, _rect.w, format, type, img);
		return (*this);
	}

	Texture const &Texture2D::parameter(GLenum pname, GLint param) const
	{
		glTexParameteri(GL_TEXTURE_2D, pname, param);
		return (*this);
	}

	Storage const &Texture2D::attachement(Framebuffer const &fbo, GLenum attach) const
	{
		glFramebufferTexture2D(fbo.getType(), attach, GL_TEXTURE_2D, _id, _levelTarget);
		return (*this);
	}

	RenderBuffer::RenderBuffer(size_t width, size_t height, GLenum internalFormat)
		: Storage(width, height, internalFormat)
	{
		glGenRenderbuffers(1, &_id);
		glBindRenderbuffer(GL_RENDERBUFFER, _id);
		glRenderbufferStorage(GL_RENDERBUFFER, internalFormat, GLsizei(width), GLsizei(height));
	}

	RenderBuffer::~RenderBuffer()
	{
		glDeleteRenderbuffers(1, &_id);
	}

	GLenum RenderBuffer::getType() const
	{
		return (GL_RENDERBUFFER);
	}

	Storage const &RenderBuffer::attachement(Framebuffer const &fbo, GLenum attach) const
	{
		glFramebufferRenderbuffer(fbo.getType(), attach, GL_RENDERBUFFER, _id);
		return (*this);
	}
}