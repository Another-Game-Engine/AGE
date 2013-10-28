#include "Framebuffer.hh"

namespace OpenGLTools
{

Framebuffer::Framebuffer()
   : _isBinded(false),
    _width(0),
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
	glBindTexture(GL_TEXTURE_2D, _depth);
	glBindRenderbuffer(GL_RENDERBUFFER, _depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void Framebuffer::bind()
{
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
	_isBinded = true;
}

void Framebuffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_isBinded = false;
}

void Framebuffer::addLayer(unsigned int id)
{
	if (_layers.find(id) != std::end(_layers))
		return;
	bind();
	GLuint tex;
	glGenTextures(1, &tex);
	//assert(tex != 0);
//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _layers.size(), GL_TEXTURE_2D, tex, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
//	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
//	glDrawBuffers(4, drawBuffers);
	_layers.insert(std::make_pair(id, tex));
	unbind();
}

void Framebuffer::bindTexture(unsigned int id)
{
	auto &e = _layers.find(id);
	if (e == std::end(_layers))
		return;
	glBindTexture(GL_TEXTURE_2D, e->second);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + id, GL_TEXTURE_2D, e->second, 0);
	glClear(GL_COLOR_BUFFER_BIT);
}
}