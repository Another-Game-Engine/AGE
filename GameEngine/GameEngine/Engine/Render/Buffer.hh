#pragma once

# include "Utils/OpenGL.hh"

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
	virtual Buffer const &BufferData(size_t size) const = 0;
	virtual Buffer const &BufferSubData(size_t offset, size_t size, void *buffer) const = 0;

	GLuint getId() const;

protected:
	GLuint _id;
};

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
	VertexBuffer const &attribute(size_t index, int nbrComponent, GLenum type, size_t offset) const;
	virtual Buffer const &BufferData(size_t size) const;
	virtual Buffer const &BufferSubData(size_t offset, size_t size, void *buffer) const;

};

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
	virtual Buffer const &BufferData(size_t size) const;
	virtual Buffer const &BufferSubData(size_t offset, size_t size, void *buffer) const;

};

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
	virtual Buffer const &BufferData(size_t size) const;
	virtual Buffer const &BufferSubData(size_t offset, size_t size, void *buffer) const;
};
