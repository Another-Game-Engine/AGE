#ifndef  __FRAMEBUFFER_HPP__
# define __FRAMEBUFFER_HPP__

#include "Utils/OpenGL.hh"

namespace OpenGLTools
{
	class Framebuffer
	{
	public:
	private:
		unsigned int _width;
		unsigned int _height;
		GLuint _handle;
		GLuint _depth;
	public:
		Framebuffer()
			: _width(0),
			_height(0),
			_handle(0),
			_depth(0)
		{}

		~Framebuffer()
		{}

		bool init(unsigned int width, unsigned int height)
		{
			_width = width;
			_height = height;
			
			glGenFrameBuffers(1, &_handle);
			glBindFrameBuffer(GL_FRAMEBUFFER, _handle);
			
			glGenRenderBuffers(1, &_depth);
			glBindRenderBuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
			glRenderBufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glBindFrameBuffer(GL_FRAMEBUFFER, 0);
		}
	private:
		Framebuffer(const Framebuffer &o);
		Framebuffer &operator=(const Framebuffer &o);
	};
};

#endif   //!__FRAMEBUFFER_HPP__