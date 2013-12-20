#ifndef VERTEXBUFFER_HH_
# define VERTEXBUFFER_HH_

# include "Utils/OpenGL.hh"

# include <iostream>
# include <vector>
# include <map>
# include <string>

# include "Attribute.hh"
# include "UniformBuffer.hh"

namespace OpenGLTools
{
	/// Buffer containing on the GPU the vertex
	class VertexBuffer
	{
	private:
		struct buffer
		{
			size_t size;
			char *data;
			buffer(size_t size, char *data)
				: size(size), data(data)
			{}
			~buffer();
		};
	private:
		GLuint _id;
		std::vector<buffer> _buffers;

	public:
		VertexBuffer();
		~VertexBuffer();
		VertexBuffer(VertexBuffer const &copy);
		VertexBuffer &operator=(VertexBuffer const &vertexbuffer);
		void init();
		void pushBuffer(char *data, size_t size);
		void popBuffer();
		void clearBuffer();
		void transferGPU() const;
	};
}

#endif /*!VERTEXBUFFER_HH_*/