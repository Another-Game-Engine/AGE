#ifdef		UNIFORMBUFFER_HH_

#include "Utils/OpenGL.hh"
#include <memory.h>

template<size_t S>
UniformBuffer<S>::UniformBuffer(GLuint bindingPoint) :
	_bindingPoint(bindingPoint),
	_globalSize(0)
{
}

template<size_t S>
UniformBuffer<S>::~UniformBuffer(void)
{
	glDeleteBuffers(1, &_bufferId);
}

template<size_t S>
void	UniformBuffer<S>::init()
{
	glGenBuffers(1, &_bufferId);
	glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _bufferId);
}

template<size_t S>
UniformBuffer<S>	&UniformBuffer<S>::registerUniform(std::string const &name, size_t offset, size_t size)
{
	SUniformVars		var;

	var.offset = offset;
	var.size = size;
	if (offset + size > _globalSize)
		_globalSize = offset + size;
	_vars[name] = var;
	return (*this);
}

template<size_t S>
void	UniformBuffer<S>::setUniform(std::string const &name, void *data)
{
	SUniformVars		var;

	var = _vars[name];
	memcpy(_buffer + var.offset, data, var.size);
}

template<size_t S>
void	UniformBuffer<S>::flushChanges()
{
	// dirty fix for @cesar AMD gpu -> to fixe later
	// www.opengl.org/discussion_boards/archive/index.php/t-178601.html
	// www.opengl.org/discussion_boards/showthread.php/178326-Uniform-Buffer-Object-Performance?p=1240435#post1240435
	glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _bufferId);
	// !end

	glBindBuffer(GL_UNIFORM_BUFFER, _bufferId);
	glBufferData(GL_UNIFORM_BUFFER, _globalSize, _buffer, GL_DYNAMIC_DRAW);
}

template<size_t S>
GLuint	UniformBuffer<S>::getBindingPoint() const
{
	return (_bindingPoint);
}

#endif