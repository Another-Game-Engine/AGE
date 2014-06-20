#pragma once

#include <OpenGL/Buffer.hh>
#include <OpenGL/MemoryGPU.hh>

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

		template <typename TYPE> UniformBlock &set(size_t index, TYPE const &value);
	private:
		bool _update;
		MemoryGPU *_data;
		size_t _nbrElement;
		size_t _sizeBlock;
		UniformBuffer _ubo;

		void GPUallocation();
	};

# define DEBUG_MESSAGE(type, from, reason, return_type) \
	{	std::cerr << std::string(type) + ": from[" + std::string(from) + "], reason[" + std::string(reason) + "]." << std::endl; return return_type; }

	template <typename TYPE>
	UniformBlock &UniformBlock::set(size_t index, TYPE const &value)
	{
		if (_update)
			GPUallocation();
		if (index >= _nbrElement)
			DEBUG_MESSAGE("Warning", "UniformBlock.hh - set(TYPE)", "index is equal or superior of nbr element into uniformBlock", *this);
		if (sizeof(TYPE) != _data[index].size)
			DEBUG_MESSAGE("Warning", "UniformBlock.hh - set(TYPE)", "the type of value size is different of fiels target into uniformBlock", *this);
		_ubo.bind();
		glBufferSubData(GL_UNIFORM_BUFFER, _data[index].offset, _data[index].size, &value);
		return (*this);
	}
}