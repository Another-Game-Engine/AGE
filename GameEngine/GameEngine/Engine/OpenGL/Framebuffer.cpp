#include <OpenGL/Framebuffer.hh>

#if TEST_ARCHI

#include <OpenGL/Texture.hh>

namespace OpenGLTools
{
	Framebuffer::Framebuffer() :
		_id(0),
		_size(0),
		_multiSample(false)
	{
	}

	Framebuffer::~Framebuffer()
	{
		if (_id != 0)
			glDeleteFramebuffers(1, &_id);
		for (auto attachment : _attachments)
			delete attachment.second;
	}

	void		Framebuffer::init(glm::uvec2 size, int sampleNbr)
	{
		if (_id != 0)
			glDeleteFramebuffers(1, &_id);
		for (auto attachement : _attachments)
			delete attachement.second;
		_attachments.clear();
		_sampleNbr = sampleNbr;
		_multiSample = sampleNbr > 1;
		_size = size;
		glGenFramebuffers(1, &_id);
	}

	void Framebuffer::addTextureAttachment(GLenum textureInternalFormat, GLenum textureFormat, GLenum attachment)
	{
		assert(_id != 0 && "Frame buffer not initialized");
		Texture *texture = NULL;
		if (_multiSample)
			texture = new TextureMultiSample(_sampleNbr, textureInternalFormat, _size.x, _size.y);
		else
		{
			float maxDimension = glm::max(static_cast<float>(_size.x), static_cast<float>(_size.y));
			texture = new Texture2D(static_cast<GLsizei>(glm::floor(glm::log2(maxDimension)) + 1), textureInternalFormat, _size.x, _size.y);
			static_cast<Texture2D *>(texture)->filter(GL_LINEAR).wrap(GL_CLAMP_TO_EDGE);

		}
		auto attach = _attachments.find(attachment);
		if (attach != _attachments.end())
		{
			delete attach->second;
			attach->second = texture;
		}
		else
			_attachments[attachment] = texture;
	}

	void		Framebuffer::attachAll() const
	{
		GLenum	texture2D = (_multiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
		bind();
		for (auto a : _attachments)
			glFramebufferTexture2D(GL_FRAMEBUFFER, a.first, texture2D, a.second->getId(), 0);
	}

	void		Framebuffer::bind() const
	{
		assert(_id != 0 && "Frame buffer not initialized");
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	Texture *Framebuffer::operator[](GLenum attachment) const
	{
		auto	attach = _attachments.find(attachment);

		if (attach != _attachments.end())
			return (attach->second);
		return (NULL);
	}
	//Texture *Framebuffer::getTextureAttachment(GLenum attachment) const
	//{
	//	auto	attach = _attachments.find(attachment);
	//
	//	if (attach != _attachments.end())
	//		return (attach->second);
	//	return (NULL);
	//}

	glm::uvec2	Framebuffer::getSize()
	{
		return (_size);
	}

}
#else

namespace OpenGLTools
{
	Framebuffer::Framebuffer() :
		_id(0),
		_size(0),
		_multiSample(false)
	{
	}

	Framebuffer::~Framebuffer()
	{
		if (_id != 0)
			glDeleteFramebuffers(1, &_id);
		for (auto a : _attachments)
			glDeleteTextures(1, &a.second);
	}

	void	Framebuffer::init(glm::uvec2 size, int sampleNbr)
	{
		if (_id != 0)
			glDeleteFramebuffers(1, &_id);
		for (auto a : _attachments)
			glDeleteTextures(1, &a.second);
		_attachments.clear();
		_sampleNbr = sampleNbr;
		_multiSample = sampleNbr > 1;
		_size = size;

		glGenFramebuffers(1, &_id);
	}

	void	Framebuffer::addTextureAttachment(GLenum textureInternalFormat, GLenum textureFormat, GLenum attachment)
	{
		assert(_id != 0 && "Frame buffer not initialized");
		GLenum	texture2D = (_multiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
		GLuint	texture;

		glGenTextures(1, &texture);
		glBindTexture(texture2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		if (_multiSample)
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, _sampleNbr, textureInternalFormat, _size.x, _size.y, GL_FALSE);
		else
		{
			float maxDimension = glm::max(static_cast<float>(_size.x), static_cast<float>(_size.y));
			glTexImage2D(GL_TEXTURE_2D, 0, textureInternalFormat, _size.x, _size.y, 0, textureFormat, GL_FLOAT, NULL);
		}
		auto	attach = _attachments.find(attachment);
		if (attach != _attachments.end())
		{
			glDeleteTextures(1, &attach->second);
			attach->second = texture;
		}
		else
			_attachments[attachment] = texture;
	}

	void	Framebuffer::attachAll() const
	{
		GLenum	texture2D = (_multiSample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D);
		bind();
		for (auto a : _attachments)
			glFramebufferTexture2D(GL_FRAMEBUFFER, a.first, texture2D, a.second, 0);
	}

	void	Framebuffer::bind() const
	{
		assert(_id != 0 && "Frame buffer not initialized");
		glBindFramebuffer(GL_FRAMEBUFFER, _id);
	}

	GLuint	Framebuffer::getTextureAttachment(GLenum attachment) const
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
#endif