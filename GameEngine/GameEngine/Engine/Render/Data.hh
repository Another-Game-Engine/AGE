#pragma once

#include <stdint.h>
#include <Render/GeometryManager.hh>
#include <Render/Pool.hh>

namespace gl
{
	class MemoryBlocksGPU;
	class Vertices;
	template<typename TYPE, std::uint32_t POOL> class Key;

	//!\file Data.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class Vertices
	//!\brief contain vertices information
	class Vertices
	{
	public:
		Vertices();
		Vertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers);
		~Vertices();
		Vertices(Vertices const &copy);
		Vertices &operator=(Vertices const &v);

		void const *getBuffer(uint8_t index) const;
		size_t getSizeBuffer(uint8_t index) const;
		uint8_t getNbrBuffers() const;
		size_t getNbrElement() const;

	private:

		void **_buffers;
		size_t *_sizeBuffers;
		size_t _nbrVertices;
		uint8_t _nbrBuffers;
	};

	//!\file Data.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class Indices
	//!\brief Indices data
	class Indices
	{
	public:
		Indices();
		Indices(Indices const &copy);
		Indices(size_t nbrIndices, uint32_t *buffer);
		Indices &operator=(Indices const &i);

		size_t getNbrElement() const;
		uint32_t const *getBuffer(size_t) const;

	private:
		size_t _nbrIndices;
		uint32_t *_buffer;
	};

}