#ifndef VERTICE_HH_
# define VERTICE_HH_

#include <stdint.h>
#include <OpenGL/GeometryManager.hh>
#include <OpenGL/Pool.hh>

namespace gl
{
	class MemoryBlocksGPU;
	class Vertices;
	template<typename TYPE> class Key;

	//!\file VerticesManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
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
		size_t getNbrVertices() const;

	private:

		void **_buffers;
		size_t *_sizeBuffers;
		size_t _nbrVertices;
		uint8_t _nbrBuffers;
	};

}

#endif /* !VERTICE_HH_ */