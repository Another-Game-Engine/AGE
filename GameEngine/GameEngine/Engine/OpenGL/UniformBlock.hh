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
	private:
		bool _update;
		MemoryGPU *_data;
		size_t _nbrElement;
		UniformBuffer _ubo;
	};

}