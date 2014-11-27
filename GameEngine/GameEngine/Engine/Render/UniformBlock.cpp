#include <Render/UniformBlock.hh>
#include <Render/Program.hh>
#include <glm/glm.hpp>

static const unsigned int nbr_type = 15;
static std::pair<size_t, GLenum> types[nbr_type] =
{
	{ sizeof(glm::mat3), GL_FLOAT_MAT3 },
	{ sizeof(glm::mat4), GL_FLOAT_MAT4 },
	{ sizeof(glm::mat2), GL_FLOAT_MAT2 },
	{ sizeof(float), GL_FLOAT },
	{ sizeof(glm::vec2), GL_FLOAT_VEC2 },
	{ sizeof(glm::vec3), GL_FLOAT_VEC3 },
	{ sizeof(glm::vec4), GL_FLOAT_VEC4 },
	{ sizeof(int), GL_INT },
	{ sizeof(glm::ivec2), GL_INT_VEC2 },
	{ sizeof(glm::ivec3), GL_INT_VEC3 },
	{ sizeof(glm::ivec4), GL_INT_VEC4 },
	{ sizeof(bool), GL_BOOL },
	{ sizeof(glm::bvec2), GL_BOOL_VEC2 },
	{ sizeof(glm::bvec3), GL_BOOL_VEC3 },
	{ sizeof(glm::bvec4), GL_BOOL_VEC4 }
};

size_t convertGLidToSize(GLint type)
{
	for (size_t index = 0; index < nbr_type; ++index)
	if (types[index].second == type)
		return (types[index].first);
	assert(0);
	return (0);
}


UniformBlock::UniformBlock()
	: _sizeBlock(0)
{
	static int bindingPoint = 0;
	_bindingPoint = bindingPoint;
	++bindingPoint;
	_sizeBlock = 0;
}

UniformBlock::~UniformBlock()
{
}

size_t UniformBlock::getNbrElement() const
{
	return (0);
	//return (_data.size());
}

size_t UniformBlock::getSizeBlock() const
{
	return (_sizeBlock);
}

GLuint UniformBlock::getBindingPoint() const
{
	return (_bindingPoint);
}

GLuint UniformBlock::getBufferId() const
{
	return (_buffer.id());
}

UniformBlock const &UniformBlock::introspection(Program const &program, GLuint indexInterfaceBlock)
{
	glGetActiveUniformBlockiv(program.getId(), indexInterfaceBlock, GL_UNIFORM_BLOCK_DATA_SIZE, &_sizeBlock);
	GLint nbrElement;
	glGetActiveUniformBlockiv(program.getId(), indexInterfaceBlock, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nbrElement);
	std::vector<GLint> elements(nbrElement);
	glGetActiveUniformBlockiv(program.getId(), indexInterfaceBlock, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, elements.data());
	for (GLint index = 0; index < nbrElement; ++index) 
	{
		if (elements[index] == -1)
			assert(0);
	}
	std::vector<GLint> offsets(nbrElement);
	std::vector<GLint> sizes(nbrElement);
	std::vector<GLint> types(nbrElement);
	std::vector<GLint> strides(nbrElement);
	glGetActiveUniformsiv(program.getId(), nbrElement, (const GLuint *)elements.data(), GL_UNIFORM_OFFSET, offsets.data());
	glGetActiveUniformsiv(program.getId(), nbrElement, (const GLuint *)elements.data(), GL_UNIFORM_SIZE, sizes.data());
	glGetActiveUniformsiv(program.getId(), nbrElement, (const GLuint *)elements.data(), GL_UNIFORM_TYPE, types.data());
	glGetActiveUniformsiv(program.getId(), nbrElement, (const GLuint *)elements.data(), GL_UNIFORM_ARRAY_STRIDE, strides.data());
	for (GLint index = 0; index < nbrElement; ++index)
	{
		//MemoryGPU m(sizes[index] * convertGLidToSize(types[index]), offsets[index], strides[index]);
		//_data.insert(m);
	}
	GPUallocation();
	return (*this);
}

void UniformBlock::GPUallocation()
{
	_buffer.bind();
	_buffer.alloc(_sizeBlock);
	glBindBufferRange(_buffer.mode(), _bindingPoint, _buffer.id(), 0, _sizeBlock);
}
