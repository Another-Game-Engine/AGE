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
		static GLuint _depth;
		OpenGLTools::VertexBuffer _vbo;
		unsigned int _layerNumber;
		unsigned int *_layers;
		std::vector<std::string> _layerNames;
	public:
		Framebuffer();
		~Framebuffer();
		bool init(unsigned int width, unsigned int height, const std::vector<std::string> &layers);
		void bindDrawTargets(GLenum *targets, unsigned int number);
		void renderBegin();
		void renderEnd();
		void Framebuffer::applyViewport();
		void renderToScreen(Shader *shader);
		inline bool isRendering(){return _isRendering;}
		void bind(Shader *shader);
		void unbind();
		void clear();
		bool checkStatus();
	private:
		Framebuffer(const Framebuffer &o);
		Framebuffer &operator=(const Framebuffer &o);
	};
}

#endif   //!__FRAMEBUFFER_HH__