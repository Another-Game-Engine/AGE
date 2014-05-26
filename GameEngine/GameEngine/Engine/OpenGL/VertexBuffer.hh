#ifndef VERTEXBUFFER_HH_
# define VERTEXBUFFER_HH_

# include "Utils/OpenGL.hh"

namespace gl
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
		VertexBuffer const &bind() const;
		VertexBuffer const &unbind() const;
		VertexBuffer &useElementArray(bool iselmentarray);
		GLuint getId() const;
		GLenum getMode() const;
	private:
		bool _isBind;
		GLenum _mode;
		GLuint _id;
	};
}

#endif /*!VERTEXBUFFER_HH_*/
