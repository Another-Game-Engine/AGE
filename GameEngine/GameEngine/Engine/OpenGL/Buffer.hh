#ifndef BUFFER_HH_
# define BUFFER_HH_

# include "Utils/OpenGL.hh"

namespace gl
{
	/// Buffer containing on the GPU the vertex
	class Buffer
	{
	public:
		Buffer();
		~Buffer();
		Buffer(Buffer const &copy);
		Buffer &operator=(Buffer const &vertexbuffer);
		virtual Buffer const &bind() const = 0;
		virtual Buffer const &unbind() const = 0;
		virtual GLenum getMode() const = 0;

		GLuint getId() const;

	protected:
		GLuint _id;
	};

	/// Buffer containing on the GPU the vertex
	class VertexBuffer : public Buffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();
		VertexBuffer(VertexBuffer const &copy);
		virtual Buffer const &bind() const;
		virtual Buffer const &unbind() const;
		virtual GLenum getMode() const;

		GLuint getId() const;
	};

	/// Buffer containing on the GPU the vertex
	class IndexBuffer : public Buffer
	{
	public:
		IndexBuffer();
		virtual ~IndexBuffer();
		IndexBuffer(IndexBuffer const &copy);
		virtual Buffer const &bind() const;
		virtual Buffer const &unbind() const;
		virtual GLenum getMode() const;

		GLuint getId() const;
	};
}

#endif