#ifndef VERTEXBUFFER_HH_
# define VERTEXBUFFER_HH_

# include "Utils/OpenGL.hh"

# include <iostream>
# include <vector>
# include <map>
# include <string>

# include "UniformBuffer.hh"

namespace OpenGLTools
{
	typedef unsigned char Byte;

	/// Buffer containing on the GPU the vertex
	class VertexBuffer
	{
	public:
		VertexBuffer(bool isIndices = false);
		~VertexBuffer();
		VertexBuffer(VertexBuffer const &copy);
		VertexBuffer &operator=(VertexBuffer const &vertexbuffer);
		void init();
		void unload();
		void bind();
		void unbind();
		GLuint getId() const;
		GLenum getMode() const;
	private:
		bool _isBind;
		GLenum _mode;
		GLuint _id;
	};
}

#endif /*!VERTEXBUFFER_HH_*/
