#pragma once

# include "Utils/OpenGL.hh"

namespace gl
{
	//!\file Buffer.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class Buffer
	//!\brief Handle Opengl Buffer Object
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

	//!\file Buffer.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class VertexBuffer
	//!\brief Handle Opengl Vertex Buffer Object
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

	//!\file Buffer.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class IndexBuffer
	//!\brief Handle Opengl Index Buffer Object
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

	//!\file Buffer.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class UniformBuffer
	//!\brief Handle Opengl Uniform Buffer Object
	class UniformBuffer : public Buffer
	{
	private:
		UniformBuffer(UniformBuffer const &copy);
	public:
		UniformBuffer();
		virtual ~UniformBuffer();

		virtual Buffer const &bind() const;
		virtual Buffer const &unbind() const;
		virtual GLenum getMode() const;
	};
}