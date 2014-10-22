#pragma once

#include <Render/Buffer.hh>
#include <Render/MemoryGPU.hh>
#include <array>
#include <cassert>
#include <set>

namespace gl
{
	class Shader;

	class UniformBlock
	{
	public:
		UniformBlock();
		~UniformBlock();
		size_t getNbrElement() const;
		size_t getSizeBlock() const;
		GLuint getBindingPoint() const;
		GLuint getBufferId() const;
		UniformBlock const &introspection(Shader const &s, GLuint indexInterfaceBlock);
		template <typename TYPE> UniformBlock &set(size_t index, TYPE const &value);
		template <typename TYPE> UniformBlock &set(size_t index, TYPE const &value, size_t indexTab);

	private:
		int _bindingPoint;
		std::set<MemoryGPU> _data;
		GLint _sizeBlock;
		UniformBuffer _buffer;

	private:
		UniformBlock(UniformBlock const &copy) = delete;
		UniformBlock &operator=(UniformBlock const &u) = delete;
		void GPUallocation();
	};

	template <typename TYPE>
	UniformBlock &UniformBlock::set(size_t index, TYPE const &value)
	{
		if (index >= _data.size())
			assert(0);
		auto &element = _data.begin();
		for (size_t start = 0; start != index; ++start)
			++element;
		if (sizeof(TYPE) != element->size)
			assert(0);
		_buffer.bind();
		glBufferSubData(GL_UNIFORM_BUFFER, element->offset, element->size, &value);
		return (*this);
	}

	template <typename TYPE>
	UniformBlock &UniformBlock::set(size_t index, TYPE const &value, size_t indexTab)
	{
		if (index >= _data.size())
			assert(0);
		auto &element = _data.begin();
		for (size_t start = 0; start != index; ++start)
			++element;
		_buffer.bind();
		glBufferSubData(GL_UNIFORM_BUFFER, element->offset + element->stride * indexTab, sizeof(TYPE), &value);
		return (*this);
	}

}