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
		unsigned int *_drawBuffers;
		std::vector<std::string> _layerNames;
	public:
		Framebuffer();
		~Framebuffer();
		bool init(unsigned int width, unsigned int height, const std::vector<std::string> &layers);
		void renderBegin();
		void renderEnd();
		void Framebuffer::applyViewport();
		void renderToScreen(Shader *shader);
		inline bool isRendering(){return _isRendering;}
		void bind(Shader *shader);
		void unbind();
		void clear();

		//template<typename... args>
		//inline void clearLayer(args&& ...)
		//{
		//	clearLayer(args...);
		//}

		//template<typename... args>
		//inline void bind(args&& ...)
		//{
		//	bind(args...);
		//}

		//template<typename... args>
		//inline void unbind(args&& ...)
		//{
		//	unbind(args...);
		//}

	private:
		Framebuffer(const Framebuffer &o);
		Framebuffer &operator=(const Framebuffer &o);
	};
}

#endif   //!__FRAMEBUFFER_HH__