#ifndef  __FRAMEBUFFER_HH__
# define __FRAMEBUFFER_HH__

#include <map>
#include <vector>
#include "Utils/OpenGL.hh"
#include "VertexBuffer.hh"

namespace OpenGLTools
{
	class Framebuffer
	{
	public:
	private:
		bool _isBinded;
		unsigned int _width;
		unsigned int _height;
		GLuint _handle;
		static GLuint _depth;
		OpenGLTools::VertexBuffer _vbo;
		unsigned int _layerNumber;
		unsigned int *_layers;
	public:
		Framebuffer();
		~Framebuffer();
		bool init(unsigned int width, unsigned int height, unsigned int layerNumber = 8);
		void bind();
		void unbind();
		void renderToScreen();
		inline bool isBinded(){return _isBinded;};
		unsigned int bindTextures();
		void unbindTextures();
		void clear(unsigned int from = 1);
	private:
		Framebuffer(const Framebuffer &o);
		Framebuffer &operator=(const Framebuffer &o);
	};
}

#endif   //!__FRAMEBUFFER_HH__