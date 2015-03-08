#include <Render/Pipelining/Buffer/Renderbuffer.hh>
#include <utility>

namespace AGE
{

	Renderbuffer::Renderbuffer(GLint width, GLint height, GLenum internal_format) :
		_id(0),
		_width(width),
		_height(height),
		_internal_format(internal_format)
	{
		glGenRenderbuffers(1, &_id);
		bind();
		glRenderbufferStorage(GL_RENDERBUFFER, _internal_format, _width, _height);
	}

	Renderbuffer::Renderbuffer(Renderbuffer &&move) :
		_id(move._id),
		_width(move._width),
		_height(move._height),
		_internal_format(move._internal_format)
	{
		move._id = 0;
	}

	Renderbuffer::~Renderbuffer()
	{
		if (_id)
		{
			glDeleteRenderbuffers(1, &_id);
		}
	}

	GLenum Renderbuffer::type() const
	{
		return (GL_RENDERBUFFER);
	}

	Renderbuffer const & Renderbuffer::bind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, _id);
		return (*this);
	}

	Renderbuffer const & Renderbuffer::unbind() const
	{
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		return (*this);
	}

	IFramebufferStorage const & Renderbuffer::attachment(Framebuffer const &framebuffer, GLenum attach) const
	{
		bind();
		glFramebufferRenderbuffer(framebuffer.type(), attach, GL_RENDERBUFFER, _id);
		unbind();
		return (*this);
	}

}