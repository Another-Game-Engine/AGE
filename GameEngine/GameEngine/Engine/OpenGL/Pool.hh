#ifndef POOL_HH_
# define POOL_HH_

#include <Utils/OpenGL.hh>
#include <OpenGL/Key.hh>
#include <OpenGL/VertexArray.hh>
#include <OpenGL/Buffer.hh>
#include <vector>
#include <stdint.h>

namespace gl
{
	class VertexPool;
	class IndexPool;
	class Vertices;
	class MemoryBlocksGPU;

	//!\file VerticesManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class VerticesPool
	//!\brief Handle one kind of vertices for the VertexManager
	class Pool
	{
	public:
		struct PoolElement
		{
			Key<MemoryBlocksGPU> memoryKey;
			Vertices const *vertices;
		};
	public:
		// constructor
		Pool();
		~Pool();
		Pool(IndexPool const &pool);
		Pool(uint8_t nbrAttribute, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent);
		Pool(Pool const &copy);
		virtual Pool &operator=(Pool const &p);

		// getter
		bool getIsIndicesPool() const;
		uint8_t getNbrAttribute() const;
		GLenum getTypeComponent(uint8_t index) const;
		uint8_t getSizeTypeComponent(uint8_t index) const;
		uint8_t getNbrComponent(uint8_t index) const;
		size_t getSizeAttribute(uint8_t index) const;
		size_t getOffsetAttribute(uint8_t index) const;
		size_t getNbrBytePool() const;
		VertexBuffer const &getVertexBuffer() const;

		// Vertices handler
		Key<PoolElement> const &addVertices(Vertices const &vertices);
		Pool &rmVertices(Key<PoolElement> const &key);
		Pool &clearPool();

		//draw and synchronisation
		virtual Pool &syncronisation() = 0;
		Pool &endSyncronisation();

	protected:

		// gl data
		gl::VertexBuffer _vbo;

		// data represent attributes
		uint8_t _nbrAttribute;
		GLenum *_typeComponent;
		uint8_t *_sizeTypeComponent;
		uint8_t *_nbrComponent;

		// data represent all vertices
		std::map<Key<MemoryBlocksGPU>, MemoryBlocksGPU> _poolMemory;
		std::map<Key<PoolElement>, PoolElement> _poolElement;
		size_t *_sizeAttribute;
		size_t *_offsetAttribute;
		size_t _nbrBytePool;
		bool _syncronized;

		//function associate to syncronisation
		void syncronizeVertices(Vertices const &vertices, MemoryBlocksGPU &memory);
	};

	class VertexPool : public Pool
	{
	public:
		VertexPool();
		VertexPool(uint8_t nbrAttribute, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent);
		VertexPool(VertexPool const &copy);
		virtual ~VertexPool();

		// attribute setter
		VertexPool &setData(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent);
		VertexPool &setNbrAttribute(uint8_t nbrAttribute);
		VertexPool &setTypeComponent(uint8_t index, GLenum type);
		VertexPool &setSizeTypeComponent(uint8_t index, uint8_t sizeType);
		VertexPool &setNbrComponent(uint8_t index, uint8_t nbrComponent);

		// tool for link index pool and  vertex pool (usefull for draw with indices ;p)
		VertexPool &attachIndexPoolToVertexPool(IndexPool const &pool);
		VertexPool &dettachIndexPoolToVertexPool();

		//draw and synchronisation
		virtual Pool &syncronisation();
		VertexPool const &draw(Key<PoolElement> const &drawWithIt, Key<PoolElement> const &drawOnIt) const;
		VertexPool const &draw(Key<PoolElement> const &drawIt) const;

	private:
		VertexArray _vao;
		IndexPool const *_indexPoolattach;
		
	};

	class IndexPool : public Pool
	{
	public:
		IndexPool();
		virtual ~IndexPool();
		IndexPool(IndexPool const &copy);
		virtual Pool &operator=(Pool const &p);
		virtual Pool &syncronisation();
		IndexPool const &draw(Key<PoolElement> const &key, MemoryBlocksGPU const &vertices) const;
		IndexPool const &draw(Key<PoolElement> const &key) const;
	};
}

#endif /*!POOL_HH_*/