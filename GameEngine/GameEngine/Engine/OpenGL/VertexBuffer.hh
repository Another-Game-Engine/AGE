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
	typedef unsigned char Byte;

	/// Buffer containing on the GPU the vertex
	class VertexBuffer
	{
	private:
		struct Buffer
		{
			size_t size;
			Byte *data;

			Buffer(size_t size, Byte *data)
				: size(size), data(data)
			{}
			~Buffer();
		};
	public:
		VertexBuffer();
		~VertexBuffer();
		VertexBuffer(VertexBuffer const &copy);
		
		VertexBuffer &operator=(VertexBuffer const &vertexbuffer);
		void pushBuffer(Byte *data, size_t size);
		
		size_t getSize() const;
		Buffer const &getBuffer(size_t index) const;
		void setBuffer(Buffer const &buffer, size_t index);
		
		void popBuffer();
		void clearBuffer();
		
		void transferGPU(bool isIndice, GLenum connect, GLenum mode) const;
		void handleError() const;

	private:
		GLuint _id;
		std::vector<Buffer> _buffers;
		size_t _size;
	};
}

#endif /*!VERTEXBUFFER_HH_*/