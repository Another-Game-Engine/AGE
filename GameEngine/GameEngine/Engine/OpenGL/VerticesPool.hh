#ifndef VERTICEPOOL_HH_
# define VERTICEPOOL_HH_

#include <Utils/OpenGL.hh>
#include <vector>
#include <stdint.h>

namespace gl
{
	class Vertices;
	class MemoryBlocksGPU;

	//!\file VerticesManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class VerticesPool
	//!\brief Handle one kind of vertices for the VertexManager
	class VerticesPool
	{
	public:
		// constructor
		VerticesPool();
		~VerticesPool();
		VerticesPool(VerticesPool const &copy);
		VerticesPool &operator=(VerticesPool const &p);

		// setter
		VerticesPool &setData(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent);
		VerticesPool &setNbrAttribute(uint8_t nbrAttribute);
		VerticesPool &setTypeComponent(uint8_t index, GLenum type);
		VerticesPool &setSizeTypeComponent(uint8_t index, uint8_t sizeType);
		VerticesPool &setNbrComponent(uint8_t index, uint8_t nbrComponent);

		// getter
		uint8_t getNbrAttribute() const;
		GLenum getTypeComponent(uint8_t index) const;
		uint8_t getSizeTypeComponent(uint8_t index) const;
		uint8_t getNbrComponent(uint8_t index) const;

		// Vertices handler
		VerticesPool &addVertices(Vertices &vertices);
		VerticesPool &rmVertices(Vertices &vertices);

	private:
		// data represent attributes
		GLenum *_typeComponent;
		uint8_t *_sizeTypeComponent;
		uint8_t *_nbrComponent;
		uint8_t _nbrAttribute;

		// data represent all vertices
		std::vector<std::pair<bool, MemoryBlocksGPU>> _pool;
		size_t _nbrBytePool;
		bool _needSync;

	};
}

#endif