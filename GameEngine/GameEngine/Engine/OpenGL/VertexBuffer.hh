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
		VertexBuffer();
		~VertexBuffer();
		VertexBuffer(VertexBuffer const &copy);
		VertexBuffer &operator=(VertexBuffer const &vertexbuffer);
		void init();
		void unload();
		void bind() const;
		void unbind() const;
		GLuint getId() const;
	private:
		GLuint _id;
	};
}

#endif /*!VERTEXBUFFER_HH_*/
