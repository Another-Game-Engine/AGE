#ifndef BUFFER_HH_
# define BUFFER_HH_

# include "Utils/OpenGL.hh"

namespace gl
{
	/// Buffer containing on the GPU the vertex
	class Buffer
	{
	private:
		Buffer(Buffer const &copy);
		Buffer &operator=(Buffer const &vertexbuffer);

	public:
		Buffer();
		~Buffer();

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
	private:
		VertexBuffer(VertexBuffer const &copy);
	public:
		VertexBuffer();
		virtual ~VertexBuffer();

		virtual Buffer const &bind() const;
		virtual Buffer const &unbind() const;
		virtual GLenum getMode() const;
	};

	/// Buffer containing on the GPU the vertex
	class IndexBuffer : public Buffer
	{
	private:
		IndexBuffer(IndexBuffer const &copy);
	public:
		IndexBuffer();
		virtual ~IndexBuffer();

		virtual Buffer const &bind() const;
		virtual Buffer const &unbind() const;
		virtual GLenum getMode() const;
	};
}

#endif