#ifndef  __FRAMEBUFFER_HH__
# define __FRAMEBUFFER_HH__

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
		Framebuffer();
		~Framebuffer();
		bool init(unsigned int width, unsigned int height);
	private:
		Framebuffer(const Framebuffer &o);
		Framebuffer &operator=(const Framebuffer &o);
	};
}

#endif   //!__FRAMEBUFFER_HH__