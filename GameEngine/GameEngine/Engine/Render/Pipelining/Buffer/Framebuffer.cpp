#include <Render/Pipelining/Buffer/Framebuffer.hh>
#include <Render/Pipelining/Buffer/IFramebufferStorage.hh>
#include <Utils/Debug.hpp>

namespace AGE
{

	Framebuffer::Framebuffer(GLint width, GLint height, GLint sample, GLenum mode)
		: _id(0),
		_mode(mode),
		_width(width),
		_height(height),
		_sample(sample)
	{
		glGenFramebuffers(1, &_id);
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_WIDTH, width);
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_HEIGHT, height);
		glFramebufferParameteri(_mode, GL_FRAMEBUFFER_DEFAULT_SAMPLES, sample);
	}

	Framebuffer::Framebuffer(Framebuffer &&move)
		: _id(move._id),
		_mode(move._mode),
		_width(move._width),
		_height(move._height),
		_sample(move._sample)
	{
		move._id = 0;
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

	Framebuffer const &Framebuffer::size(GLint width, GLint height, GLint sample)
	{
		_width = width;
		_height = height;
		_sample = sample;
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
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			AGE_ASSERT(false);
		}
		return (*this);
	}

}