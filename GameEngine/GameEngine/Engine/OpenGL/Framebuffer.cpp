#include <memory>
#include "Framebuffer.hh"
#include "Shader.hh"

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

bool Framebuffer::init(unsigned int width, unsigned int height, const std::vector<std::string> &layerNames)
{
	// todo clear all if allready initilized

	_width = width;
	_height = height;

	_layerNumber = layerNames.size();
	_layerNames = layerNames;
	glGenFramebuffers(1, &_handle);
	glBindFramebuffer(GL_FRAMEBUFFER, _handle);
	
	_layers = new unsigned int[_layerNumber];
	glGenTextures(_layerNumber, _layers);

	for (unsigned int i = 0; i < _layerNumber; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, _layers[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _layers[i], 0);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
 
	_drawBuffers = new unsigned int[_layerNumber];
	for (size_t c = 0; c < _layerNumber; ++c)
		_drawBuffers[c] = GL_COLOR_ATTACHMENT0 + c;
	glDrawBuffers(_layerNumber, _drawBuffers);

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
	glDrawBuffers(_layerNumber, _drawBuffers);
	_isRendering = true;
}

void Framebuffer::renderEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	_isRendering = false;
}

void Framebuffer::applyViewport()
{
	glViewport(0, 0, _width, _height);
}

void Framebuffer::bind(Shader *shader)
{
	for (unsigned int i = 0; i < _layerNumber; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _layers[i]);
//		shader->bindActiveTexture(_layerNames[i], _layers[i]);
	}
}

void Framebuffer::unbind()
{
	for (unsigned int i = 0; i < _layerNumber; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Framebuffer::clear()
{
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Framebuffer::renderToScreen(Shader *shader)
{
	shader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _layers[0]);
//	shader->bindActiveTexture(_layerNames[0], 1);
	_vbo.draw(GL_TRIANGLES);
}

}