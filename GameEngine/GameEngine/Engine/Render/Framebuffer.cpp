#include <Render/Framebuffer.hh>

namespace gl
{
	Framebuffer::Framebuffer(GLenum mode, GLint width, GLint height, GLint sampler)
		: _id(0),
		_mode(mode)
	{
		glGenFramebuffers(1, &_id);
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_SAMPLES, sampler);
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
}