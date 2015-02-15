#include <Render/Pipelining/Buffer/Framebuffer.hh>
#include <Render/Pipelining/Buffer/IFramebufferStorage.hh>

namespace AGE
{

	Framebuffer::Framebuffer(GLenum mode)
		: _id(0),
		_mode(mode)
	{
		glGenFramebuffers(1, &_id);
	}

	Framebuffer::~Framebuffer()
	{
		if (_id)
			glDeleteFramebuffers(1, &_id);
	}

	Framebuffer const &Framebuffer::bind() const
	{
		glBindFramebuffer(_mode, _id);
		return (*this);
	}

	Framebuffer const &Framebuffer::unbind() const
	{
		glBindFramebuffer(_mode, 0);
		return (*this);
	}

	Framebuffer const &Framebuffer::size(GLint width, GLint height, GLint sample) const
	{
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_SAMPLES, sample);
		return (*this);
	}

	GLuint Framebuffer::id() const
	{
		return (_id);
	}

	GLenum Framebuffer::type() const
	{
		return (_mode);
	}

	Framebuffer const &Framebuffer::attachment(IFramebufferStorage const &storage, GLenum attach) const
	{
		storage.attachment(*this, attach);
		return (*this);
	}

}