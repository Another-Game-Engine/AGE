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
		bool _isRendering;
		unsigned int _width;
		unsigned int _height;
		GLuint _handle;
		GLuint _depth;
		OpenGLTools::VertexBuffer _vbo;
		unsigned int _layerNumber;
		unsigned int *_layers;
		std::vector<std::string> _layerNames;
	public:
		Framebuffer();
		~Framebuffer();
		bool init(unsigned int width, unsigned int height, unsigned int layerNumber);
		void uninit();
		void bindDrawTargets(GLenum *targets, unsigned int number);
		void zPassBegin();
		void zPassEnd();
		void renderBegin();
		void renderEnd();
		void Framebuffer::applyViewport();
		void renderRect(Shader *shader);
		void debugRendering(Shader *shader);
		bool isRendering();
		unsigned int bind(Shader *shader);
		void unbind();
		void clearColor();
		void clearZ();
		bool checkStatus();
	private:
		Framebuffer(const Framebuffer &o);
		Framebuffer &operator=(const Framebuffer &o);
	};
}

#endif   //!__FRAMEBUFFER_HH__