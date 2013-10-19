#ifdef		UNIFORMBUFFER_HH_

#include "Utils/OpenGL.hh"
#include <memory.h>

template<size_t S>
UniformBuffer::UniformBuffer(GLuint bindingPoint) :
	_bindingPoint(bindingPoint) :
	_globalSize(0)
{
}

template<size_t S>
UniformBuffer::~UniformBuffer(void)
{
	glDeleteBuffers(1, &_bufferId);
}

template<size_t S>
void	UniformBuffer::init()
{
	glGenBuffers(1, &_bufferId);
	glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _bufferId);
}

template<size_t S>
void	UniformBuffer::registerUniform(std::string const &name, size_t offset, size_t size)
{
	SUniformVars		var;

	var.offset = offset;
	var.size = size;
	if (offset + size > _globalSize)
		_globalSize = offset + size;
	_vars[name] = var;
}

template<size_t S>
void	UniformBuffer::setUniform(std::string const &name, void *data)
{
	SUniformVars		var;

	var = _vars[name];
	memcpy(_buffer + var.offset, data, var.size);
}

template<size_t S>
void	UniformBuffer::flushChanges()
{
	glBindBuffer(GL_UNIFORM_BUFFER, _bufferId);
	glBufferData(GL_UNIFORM_BUFFER, _globalSize, _buffer, GL_DYNAMIC_DRAW);
}

template<size_t S>
GLuint	UniformBuffer::getBindingPoint() const
{
	return (_bindingPoint);
}

#endif