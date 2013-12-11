#include <memory>
#include <cmath>
#include "Framebuffer.hh"
#include "Shader.hh"

namespace OpenGLTools
{
	Framebuffer::Framebuffer()
		: _isRendering(false),
		_width(0),
		_height(0),
		_handle(0),
		_depth(0),
		_layerNumber(0),
		_layers(nullptr)
	{}

	Framebuffer::~Framebuffer()
	{
		uninit();
	}

	bool Framebuffer::init(unsigned int width, unsigned int height, unsigned int layerNumber)
	{
		// todo clear all if allready initilized

		_width = width;
		_height = height;

		_layerNumber = layerNumber;
		glGenFramebuffers(1, &_handle);
		glBindFramebuffer(GL_FRAMEBUFFER, _handle);

		_layers = new unsigned int[_layerNumber];
		glGenTextures(_layerNumber, _layers);

		for (unsigned int i = 0; i < _layerNumber; ++i)
		{
			_layerNames.push_back("layer" + std::to_string(i));
			glBindTexture(GL_TEXTURE_2D, _layers[i]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)(GL_COLOR_ATTACHMENT0 + i), GL_TEXTURE_2D, _layers[i], 0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		glGenRenderbuffers(1, &_depth);

		glBindRenderbuffer(GL_RENDERBUFFER, _depth);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _width, _height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		if (!checkStatus())
			return false;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);


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
		unsigned int indice[] = {0,1,2,3,4,5};
		_vbo.init(6, &indice[0]);
		_vbo.addAttribute(OpenGLTools::Attribute(sizeof(float) * 2, 2, GL_FLOAT));
		_vbo.addAttribute(OpenGLTools::Attribute(sizeof(float) * 2, 2, GL_FLOAT));
		_vbo.setBuffer(0, reinterpret_cast<byte *>(&ss_quad_pos));
		_vbo.setBuffer(1, reinterpret_cast<byte *>(&ss_quad_st));

		return true;
	}

	void Framebuffer::uninit()
	{
		if (_layers)
		{
			glDeleteTextures(_layerNumber, _layers);
			delete _layers;
			_layers = nullptr;
		}
		if (_depth != 0)
		{
			glDeleteRenderbuffers(1, &_depth);
			_depth = 0;
		}
		if (_handle != 0)
		{
			glDeleteFramebuffers(1, &_handle);
			_handle = 0;
		}
	}

	void Framebuffer::bindDrawTargets(GLenum *targets, unsigned int number)
	{
		glDrawBuffers(number, targets);
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

	void Framebuffer::zPassBegin()
	{
		glDepthFunc(GL_LESS);
		glColorMask(0,0,0,0);
		glDepthMask(GL_TRUE);
	}

	void Framebuffer::zPassEnd()
	{
		glDepthFunc(GL_LEQUAL);
		glColorMask(1,1,1,1);
//		glDepthMask(GL_FALSE);
	}

	void Framebuffer::applyViewport()
	{
		glViewport(0, 0, _width, _height);
	}

	unsigned int Framebuffer::bind(Shader *shader)
	{
		static GLenum activeTexture = GL_TEXTURE0;
		unsigned int i = 0;
		for (auto &e : shader->getLayers())
		{
			if (GL_TEXTURE0 + i != activeTexture)
				glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, _layers[e - GL_COLOR_ATTACHMENT0]);
			++i;
		}
		activeTexture = GL_TEXTURE0 + (i - 1);
		return _layerNumber;
	}

	void Framebuffer::unbind()
	{
		for (unsigned int i = 0; i < _layerNumber; ++i)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void Framebuffer::clearColor()
	{
		std::unique_ptr<GLenum[]> drawBuffers(new GLenum[_layerNumber]);
		for (unsigned int i = 0; i < _layerNumber; ++i)
		{
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(_layerNumber, drawBuffers.get());
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Framebuffer::clearZ()
	{
		glDrawBuffers(0, nullptr);
		glClear(GL_DEPTH_BUFFER_BIT);
	}


	void Framebuffer::renderRect(Shader *shader)
	{
		shader->use();

		glViewport(0,0,_width,_height);
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, _layers[0]);
		bind(shader);
		_vbo.draw(GL_TRIANGLES);

		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Framebuffer::debugRendering(Shader *shader)
	{
		shader->use();
		for (unsigned int i = 0; i < _layerNumber; ++i)
		{
			glViewport(
				(i % 3) * ((float)_width / 3),
				_height - ((i / 3 + 1) * ((float)_height / 3)),
				(float)_width / 3,
				(float)_height / 3
				);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, _layers[i]);
			_vbo.draw(GL_TRIANGLES);
		   
		}
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	bool Framebuffer::checkStatus()
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		switch(status)
		{
		case GL_FRAMEBUFFER_COMPLETE:
			std::cout << "Framebuffer complete." << std::endl;
			return true;

		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			std::cerr << "[ERROR] Framebuffer incomplete: Attachment is NOT complete." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			std::cerr << "[ERROR] Framebuffer incomplete: No image is attached to FBO." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			std::cerr << "[ERROR] Framebuffer incomplete: Draw buffer." << std::endl;
			return false;

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			std::cerr << "[ERROR] Framebuffer incomplete: Read buffer." << std::endl;
			return false;

		case GL_FRAMEBUFFER_UNSUPPORTED:
			std::cerr << "[ERROR] Framebuffer incomplete: Unsupported by FBO implementation." << std::endl;
			return false;

		default:
			std::cerr << "[ERROR] Framebuffer incomplete: Unknown error." << std::endl;
			return false;
		}	
	}

	bool Framebuffer::isRendering()
	{
		return _isRendering;
	}
}