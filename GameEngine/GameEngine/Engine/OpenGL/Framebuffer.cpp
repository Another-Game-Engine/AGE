#include <memory>
#include <cmath>
#include "Framebuffer.hh"
#include "Shader.hh"

namespace OpenGLTools
{
	Framebuffer::Framebuffer() :
		_id(0),
		_size(0)
	{
	}

	Framebuffer::~Framebuffer()
	{
		if (_id != 0)
			glDeleteFramebuffers(1, &_id);
		for (auto a : _attachments)
			glDeleteTextures(1, &a.second);
	}

	void		Framebuffer::init(glm::uvec2 size)
	{
		_size = size;
		glGenFramebuffers(1, &_id);
	}

	void		Framebuffer::addTextureAttachment(GLenum textureInternalFormat, GLenum textureFormat, GLenum attachment)
	{
		assert(_id != 0 && "Frame buffer not initialized");
		GLuint	texture;

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, textureInternalFormat, _size.x, _size.y, 0, textureFormat, GL_FLOAT, NULL);
		auto	attach = _attachments.find(attachment);
		if (attach != _attachments.end())
		{
			glDeleteTextures(1, &attach->second);
			attach->second = texture;
		}
		else
			_attachments[attachment] = texture;
	}

	void		Framebuffer::attachAll() const
	{
		bind();
		for (auto a : _attachments)
			glFramebufferTexture2D(GL_FRAMEBUFFER, a.first, GL_TEXTURE_2D, a.second, 0);
	}

	void		Framebuffer::bind() const
	{
		assert(_id != 0 && "Frame buffer not initialized");
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	GLuint		Framebuffer::getTextureAttachment(GLenum attachment) const
	{
		auto	attach = _attachments.find(attachment);

		if (attach != _attachments.end())
		{
			return (attach->second);
		}
		return (-1);
	}

	glm::uvec2	Framebuffer::getSize()
	{
		return (_size);
	}

}