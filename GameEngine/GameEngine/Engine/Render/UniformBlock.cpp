#include <Render/UniformBlock.hh>
#include <Render/Shader.hh>

namespace gl
{
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


	UniformBlock::UniformBlock(UniformBlock const &copy)
		: _bindingPoint(copy._bindingPoint),
		_sizeBlock(copy._sizeBlock),
		_data(copy._data)
	{
	}

	UniformBlock &UniformBlock::operator=(UniformBlock const &u)
	{
		if (this != &u)
		{
			_sizeBlock = u._sizeBlock;
			_bindingPoint = u._bindingPoint;
			_data = u._data;
		}
		return (*this);
	}

	size_t UniformBlock::getNbrElement() const
	{
		return (_data.size());
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
		return (_buffer.getId());
	}

	static size_t convertEnumToSize(GLint typeEnum)
	{
		switch (typeEnum)
		{
		case GL_FLOAT_MAT3:
			return (sizeof(glm::mat3));
		case GL_FLOAT_MAT4:
			return (sizeof(glm::mat4));
		case GL_FLOAT:
			return (sizeof(float));
		case GL_FLOAT_VEC3:
			return (sizeof(glm::vec3));
		case GL_FLOAT_VEC4:
			return (sizeof(glm::vec4));
		case GL_INT:
			return (sizeof(int));
		case  GL_FLOAT_VEC2:
			return (sizeof(glm::vec2));
		case GL_INT_VEC2:
			return (sizeof(glm::ivec2));
		case GL_INT_VEC3:
			return (sizeof(glm::ivec3));
		case GL_INT_VEC4:
			return (sizeof(glm::ivec4));
		case GL_BOOL:
			return (sizeof(bool));
		case GL_BOOL_VEC2:
			return (sizeof(glm::bvec2));
		case GL_BOOL_VEC3:
			return (sizeof(glm::bvec3));
		case GL_BOOL_VEC4:
			return (sizeof(glm::bvec4));
		case GL_FLOAT_MAT2:
			return (sizeof(glm::mat2));
		default:
			assert(0);
		}
		return (0);
	}

	UniformBlock const &UniformBlock::introspection(Shader const &s, GLuint indexInterfaceBlock)
	{
		glGetActiveUniformBlockiv(s.getId(), indexInterfaceBlock, GL_UNIFORM_BLOCK_DATA_SIZE, &_sizeBlock);
		GLint nbrElement;
		glGetActiveUniformBlockiv(s.getId(), indexInterfaceBlock, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &nbrElement);
		GLint *elements = new GLint[nbrElement];
		glGetActiveUniformBlockiv(s.getId(), indexInterfaceBlock, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, elements);
		for (GLint index = 0; index < nbrElement; ++index)
		{
			if (elements[index] == -1)
				assert(0);
		}
		GLint *offsets = new GLint[nbrElement];
		GLint *sizes = new GLint[nbrElement];
		GLint *types = new GLint[nbrElement];
		GLint *strides = new GLint[nbrElement];
		glGetActiveUniformsiv(s.getId(), nbrElement, (const GLuint *)elements, GL_UNIFORM_OFFSET, offsets);
		glGetActiveUniformsiv(s.getId(), nbrElement, (const GLuint *)elements, GL_UNIFORM_SIZE, sizes);
		glGetActiveUniformsiv(s.getId(), nbrElement, (const GLuint *)elements, GL_UNIFORM_TYPE, types);
		glGetActiveUniformsiv(s.getId(), nbrElement, (const GLuint *)elements, GL_UNIFORM_ARRAY_STRIDE, strides);
		for (GLint index = 0; index < nbrElement; ++index)
		{
			MemoryGPU m(sizes[index] * convertEnumToSize(types[index]), offsets[index], strides[index]);
			_data.insert(m);
		}
		delete[] offsets;
		delete[] sizes;
		delete[] types;
		delete[] strides;
		GPUallocation();
		return (*this);
	}

	void UniformBlock::GPUallocation()
	{
		_buffer.bind();
		_buffer.BufferData(_sizeBlock);
		glBindBufferRange(_buffer.getMode(), _bindingPoint, _buffer.getId(), 0, _sizeBlock);
	}
}