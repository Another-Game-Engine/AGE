#ifndef POOL_HH_
# define POOL_HH_

#include <Utils/OpenGL.hh>
#include <OpenGL/Key.hh>
#include <OpenGL/VertexArray.hh>
#include <OpenGL/Buffer.hh>
#include <map>
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
			size_t memoryIndex;
			Vertices const *vertices;
		};
	public:
		// constructor
		Pool();
		~Pool();
		Pool(int);
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
		

		// Vertices handler
		Key<PoolElement> addVertices(Vertices const &vertices);
		Pool &rmVertices(Key<PoolElement> const &key);
		Pool &clearPool();

		//draw and synchronisation
		virtual Pool &syncronisation() = 0;
		virtual Buffer const &getBuffer() const = 0;

	protected:

		// data represent attributes
		uint8_t _nbrAttribute;
		GLenum *_typeComponent;
		uint8_t *_sizeTypeComponent;
		uint8_t *_nbrComponent;

		// data represent all vertices
		std::vector<MemoryBlocksGPU> _poolMemory;
		std::map<Key<PoolElement>, PoolElement> _poolElement;
		size_t *_sizeAttribute;
		size_t _nbrElementPool;
		size_t *_offsetAttribute;
		size_t _nbrBytePool;
		bool _syncronized;
		bool _internalSyncronized;

		//function associate to syncronisation
		void syncronizeVertices(GLenum mode, Vertices const &vertices, MemoryBlocksGPU &memory);
	};

	class VertexPool : public Pool
	{
	public:
		VertexPool();
		VertexPool(uint8_t nbrAttribute, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent);
		VertexPool(VertexPool const &copy);
		virtual ~VertexPool();
		VertexPool &operator=(VertexPool const &pool);

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
		virtual Buffer const &getBuffer() const;
		VertexPool const &draw(GLenum mode, Key<PoolElement> const &drawWithIt, Key<PoolElement> const &drawOnIt) const;
		VertexPool const &draw(GLenum mode, Key<PoolElement> const &drawIt) const;

	private:
		VertexBuffer _vbo;
		VertexArray _vao;
		IndexPool const *_indexPoolattach;
		
	};

	class IndexPool : public Pool
	{
	public:
		IndexPool();
		IndexPool(IndexPool const &copy);
		virtual ~IndexPool();
		virtual IndexPool &operator=(IndexPool const &p);
		
		virtual Pool &syncronisation();
		virtual Buffer const &getBuffer() const;

		IndexPool const &draw(GLenum mode, Key<PoolElement> const &key) const;
		IndexPool const &draw(GLenum mode, Key<PoolElement> const &key, MemoryBlocksGPU const &target) const;
	
	private:
		gl::IndexBuffer _ibo;

	};
}

#endif /*!POOL_HH_*/