#include "Framebuffer.hh"

namespace OpenGLTools
{

Framebuffer::Framebuffer()
   : _width(0),
	_height(0),
	_handle(0),
	_depth(0)
{}

Framebuffer::~Framebuffer()
{}

bool Framebuffer::init(unsigned int width, unsigned int height)
{
	_width = width;
	_height = height;
	glGenFramebuffers(1, &_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
			
	glGenRenderbuffers(1, &_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, _depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}