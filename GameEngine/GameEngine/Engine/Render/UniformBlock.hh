#pragma once

#include <Render/Buffer.hh>
#include <Render/MemoryGPU.hh>
#include <array>
#include <cassert>

namespace gl
{

	//!\file GeometryManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class GeometryManager
	//!\brief Handle the geometry of the render
	class UniformBlock
	{
	public:
		UniformBlock();
		UniformBlock(size_t nbrElement, size_t *sizeElement);
		~UniformBlock();
		UniformBlock(UniformBlock const &copy);
		UniformBlock &operator=(UniformBlock const &u);

		size_t getNbrElement() const;
		size_t getSizeElement(size_t index) const;
		size_t getOffsetElement(size_t index) const;
		size_t getSizeBlock(size_t index) const;
		GLuint getBindingPoint() const;
		GLuint getBufferId() const;
		UniformBlock const &update() const;

		template <typename TYPE> UniformBlock &set(size_t index, TYPE const &value);
	
	private:
		bool _update;
		int _bindingPoint;
		MemoryGPU *_data;
		size_t _nbrElement;
		size_t _sizeBlock;
		UniformBuffer _buffer;

		void GPUallocation();
	};

	template <typename TYPE>
	UniformBlock &UniformBlock::set(size_t index, TYPE const &value)
	{
		if (_update)
			GPUallocation();
		if (index >= _nbrElement)
			assert(0);
		if (sizeof(TYPE) != _data[index].size)
			assert(0);
		_buffer.bind();
		glBufferSubData(GL_UNIFORM_BUFFER, _data[index].offset, _data[index].size, &value);
		return (*this);
	}

}