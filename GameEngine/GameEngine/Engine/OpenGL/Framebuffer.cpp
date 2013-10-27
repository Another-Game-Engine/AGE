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
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _handle);
			
	glGenTextures(1, &_depth);
	glBindTexture(GL_TEXTURE_2D, _depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth, 0);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
//	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _handle, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void Framebuffer::bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, _depth);
}

void Framebuffer::unbind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::addLayer(unsigned int id)
{
	if (_layers.find(id) != std::end(_layers))
		return;
	bind();
	GLuint tex;
	glGenTextures(1, &tex);
	//assert(tex != 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _layers.size(), GL_TEXTURE_2D, tex, 0);
//	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
//	glDrawBuffers(4, drawBuffers);

	glBindTexture(GL_TEXTURE_2D, 0);
	_layers.insert(std::make_pair(id, tex));
	unbind();
}

void Framebuffer::bindTexture(unsigned int id)
{
	auto &e = _layers.find(id);
	if (e == std::end(_layers))
		return;
}

}