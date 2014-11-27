#pragma once

# include <Utils/OpenGL.hh>

class IBuffer
{
public:
	virtual IBuffer const &bind() const = 0;
	virtual IBuffer const &unbind() const = 0;
	virtual GLenum mode() const = 0;
	virtual IBuffer const &alloc(size_t size) const = 0;
	virtual IBuffer const &sub(size_t offset, size_t size, void const *buffer) const = 0;
};