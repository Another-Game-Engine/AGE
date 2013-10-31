#include <memory>
#include "Framebuffer.hh"

namespace OpenGLTools
{

unsigned int Framebuffer::_depth = 0;

Framebuffer::Framebuffer()
   : _isRendering(false),
    _width(0),
	_height(0),
	_handle(0),
	_layerNumber(0),
	_layers(nullptr)
{}

Framebuffer::~Framebuffer()
{
}

bool Framebuffer::init(unsigned int width, unsigned int height, unsigned int layerNumber)
{
	// todo clear all if allready initilized

	_width = width;
	_height = height;

	_layerNumber = layerNumber;
	glGenFramebuffers(1, &_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
	
	_layers = new unsigned int[layerNumber];
	glGenTextures(layerNumber, _layers);

	for (unsigned int i = 0; i < layerNumber; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, _layers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _layers[i], 0);
	}
		glBindTexture(GL_TEXTURE_2D, 0);
 
	std::unique_ptr<GLenum[]> arr(new GLenum[_layerNumber]);
	for (size_t c = 0; c < _layerNumber; ++c)
		arr[c] = GL_COLOR_ATTACHMENT0 + c;
	glDrawBuffers(_layerNumber, arr.get());

	if (_depth == 0)
	{
		glGenRenderbuffers(1, &_depth);
	}

	glBindRenderbuffer(GL_RENDERBUFFER, _depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, _width, _height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);

	// x,y vertex positions
	float ss_quad_pos[] = {
		-1.0, -1.0,
		1.0, -1.0,
		1.0,  1.0,
		1.0,  1.0,
		-1.0,  1.0,
		-1.0, -1.0
	};
	// per-vertex texture coordinates
	float ss_quad_st[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		1.0, 1.0,
		0.0, 1.0,
		0.0, 0.0
	};
	unsigned int indice[] = {0,1,2,3,4,5,6};
	_vbo.init(6, &indice[0]);
	_vbo.addAttribute(OpenGLTools::Attribute(sizeof(float) * 2, 2, GL_FLOAT));
	_vbo.addAttribute(OpenGLTools::Attribute(sizeof(float) * 2, 2, GL_FLOAT));
	_vbo.setBuffer(0, reinterpret_cast<byte *>(&ss_quad_pos));
	_vbo.setBuffer(1, reinterpret_cast<byte *>(&ss_quad_st));

	return true;
}

void Framebuffer::renderBegin()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
	_isRendering = true;
}

void Framebuffer::renderEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_isRendering = false;
}

void Framebuffer::bind(unsigned int target)
{
	glActiveTexture(GL_TEXTURE0 + target);
	glBindTexture(GL_TEXTURE_2D, _layers[target]);
}

void Framebuffer::unbind(unsigned int layer)
{
	glActiveTexture(GL_TEXTURE0 + layer);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::clearDepth()
{
	glClear(GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::clearLayer(unsigned int layer)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _layers[layer]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Framebuffer::renderToScreen()
{
	_vbo.draw(GL_TRIANGLES);
}

}