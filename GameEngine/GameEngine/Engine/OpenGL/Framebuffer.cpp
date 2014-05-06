#include <OpenGL/Framebuffer.hh>
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
			texture = new Texture2D(0, textureInternalFormat, _size.x, _size.y);
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