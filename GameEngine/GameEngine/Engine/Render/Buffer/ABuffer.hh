#pragma once

#include <string>
#include<Utils/OpenGL.hh>

class ABuffer
{
public:
	virtual ~ABuffer();
	ABuffer(ABuffer const &copy) = delete;
	ABuffer &operator=(ABuffer const &i) = delete;

protected:
	ABuffer();
	ABuffer(ABuffer &&move);
	virtual void _bind() = 0;
	virtual void _unbind() = 0;
public:
	GLuint id() const;
	size_t size() const;
	virtual GLenum mode() const = 0;
	virtual ABuffer const &alloc(size_t size) = 0;
	virtual ABuffer const &sub(size_t offset, size_t size, void const *buffer) const = 0;
	void bind();
	void unbind();
	bool isBinded() const;
protected:
	GLuint _id;
	size_t _size;
	bool _binded;
};