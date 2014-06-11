
#include "UniformBuffer.hh"
#include "Shader.hh"
#include "Utils/OpenGL.hh"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <list>
#include <cstdint>

namespace	OpenGLTools
{

UniformBuffer::UniformBuffer(GLuint bindingPoint) :
	_bindingPoint(bindingPoint),
	_bufferId(0),
	_dataSize(0),
	_buffer(NULL)
{
}

UniformBuffer::~UniformBuffer(void)
{
	glDeleteBuffers(1, &_bufferId);
	if (_buffer)
		delete[] _buffer;
}

void	UniformBuffer::init(std::shared_ptr<gl::Shader> referent, std::string const &blockName, std::uint32_t bufferSize)
{
	glGenBuffers(1, &_bufferId);
	glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _bufferId);

	GLint	blockIdx = glGetUniformBlockIndex(referent->getId(), blockName.c_str());
	// find the total size to check if the size passed as a parameter is correct
	glGetActiveUniformBlockiv(referent->getId(), blockIdx, GL_UNIFORM_BLOCK_DATA_SIZE, (GLint*)&_dataSize);

	_dataSize = glm::max(_dataSize, bufferSize);
	_buffer = new char[_dataSize];

}

void	UniformBuffer::setBufferData(size_t size, const char *data)
{
	assert(size <= _dataSize && "Size to big for this uniform buffer");
	memcpy(_buffer, data, size);
}

void	UniformBuffer::init(std::shared_ptr<gl::Shader> referent, std::string const &blockName, std::string const vars[])
{
	GLint		blockIdx, varNbr;
	GLint		*varsInfos;

	glGenBuffers(1, &_bufferId);
	glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _bufferId);
	blockIdx = glGetUniformBlockIndex(referent->getId(), blockName.c_str());
	// find the total size to allocate the buffer
	glGetActiveUniformBlockiv(referent->getId(), blockIdx,	GL_UNIFORM_BLOCK_DATA_SIZE, (GLint*)&_dataSize);
	_buffer = new char[_dataSize];
	// get the number of uniforms
	glGetActiveUniformBlockiv(referent->getId(), blockIdx,	GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &varNbr);
	assert(varNbr > 0 && "glGetActiveUniformBlockid Error");
	// we store the uniforms informations in this table with this layout:
	// Indices - Types - Offset
	varsInfos = new GLint[varNbr * 3];
	glGetActiveUniformBlockiv(referent->getId(), blockIdx,	GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, varsInfos);
	glGetActiveUniformsiv(referent->getId(), varNbr, (GLuint*)varsInfos, GL_UNIFORM_TYPE, varsInfos + varNbr);
	glGetActiveUniformsiv(referent->getId(), varNbr, (GLuint*)varsInfos, GL_UNIFORM_OFFSET, varsInfos + 2 * varNbr);
	// sort the vars by offset to make them correspond with the order of the names
	std::list<std::pair<GLint, GLint> >				sorted;
	std::list<std::pair<GLint, GLint> >::iterator	it;
	GLint											curOffset;
	GLint											i;

	// insertion sort
	for (i = 0; i < varNbr; ++i)
	{
		curOffset = varsInfos[varNbr * 2 + i];
		it = sorted.begin();
		while (it != sorted.end() && it->second < curOffset)
			++it;
		sorted.insert(it, std::pair<GLint, GLint>(i, curOffset));
	}
	// bind the name with the offset and type
	it = sorted.begin();
	for (i = 0; it != sorted.end() && i < varNbr; ++i)
	{
		SUniformVars		added;
	
		added.offset = varsInfos[varNbr * 2 + it->first];
		added.type = varsInfos[varNbr + it->first];
		_vars[vars[i]] = added;
		++it;
	}
	delete[] varsInfos;
}

void	UniformBuffer::setUniform(std::string const &name, int data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_INT);
	memcpy(_buffer + it->second.offset, &data, sizeof(int));
}

void	UniformBuffer::setUniform(std::string const &name, unsigned int data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_UNSIGNED_INT);
	memcpy(_buffer + it->second.offset, &data, sizeof(unsigned int));
}

void	UniformBuffer::setUniform(std::string const &name, float data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_FLOAT);
	memcpy(_buffer + it->second.offset, &data, sizeof(float));
}

void	UniformBuffer::setUniform(std::string const &name, glm::vec2 const &data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_FLOAT_VEC2);
	memcpy(_buffer + it->second.offset, glm::value_ptr(data), 2 * sizeof(float));
}

void	UniformBuffer::setUniform(std::string const &name, glm::vec3 const &data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_FLOAT_VEC3);
	memcpy(_buffer + it->second.offset, glm::value_ptr(data), 3 * sizeof(float));
}

void	UniformBuffer::setUniform(std::string const &name, glm::vec4 const &data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_FLOAT_VEC4);
	memcpy(_buffer + it->second.offset, glm::value_ptr(data), 4 * sizeof(float));
}

void	UniformBuffer::setUniform(std::string const &name, glm::mat2 const &data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_FLOAT_MAT2);
	memcpy(_buffer + it->second.offset, glm::value_ptr(data), 2 * 2 * sizeof(float));
}

void	UniformBuffer::setUniform(std::string const &name, glm::mat3 const &data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_FLOAT_MAT3);
	memcpy(_buffer + it->second.offset, glm::value_ptr(data), 3 * 3 * sizeof(float));
}

void	UniformBuffer::setUniform(std::string const &name, glm::mat4 const &data)
{
	std::map<std::string, SUniformVars>::iterator	it = _vars.find(name);
	assert(_buffer && it != _vars.end() && it->second.type == GL_FLOAT_MAT4);
	memcpy(_buffer + it->second.offset, glm::value_ptr(data), 4 * 4 * sizeof(float));
}

void	UniformBuffer::flushChanges()
{
	// dirty fix for @cesar AMD gpu -> to fixe later
	// www.opengl.org/discussion_boards/archive/index.php/t-178601.html
	// www.opengl.org/discussion_boards/showthread.php/178326-Uniform-Buffer-Object-Performance?p=1240435#post1240435
	glBindBufferBase(GL_UNIFORM_BUFFER, _bindingPoint, _bufferId);
	// !end

	glBindBuffer(GL_UNIFORM_BUFFER, _bufferId);
	glBufferData(GL_UNIFORM_BUFFER, _dataSize, _buffer, GL_DYNAMIC_DRAW);
}

GLuint	UniformBuffer::getBindingPoint() const
{
	return (_bindingPoint);
}

}