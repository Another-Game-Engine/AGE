#pragma once

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
	class Indices;
	class MemoryBlocksGPU;

	//!\file Pool.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Pool
	//!\brief Handle one kind of element for the GeometryManager
	class Pool
	{
	public:
		template <typename TYPE>
		struct Element
		{
			size_t memoryIndex;
			TYPE const *data;
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

		//draw and synchronisation
		virtual Pool &syncronisation() = 0;
		virtual Buffer const &getBuffer() const = 0;

	protected:

		// data represent attributes
		uint8_t _nbrAttribute;
		GLenum *_typeComponent;
		uint8_t *_sizeTypeComponent;
		uint8_t *_nbrComponent;
		size_t *_sizeAttribute;
		size_t *_offsetAttribute;

		// represent all data
		std::vector<MemoryBlocksGPU> _poolMemory;
		size_t _nbrBytePool;
		size_t _nbrElementPool;
		bool _syncronized;
		bool _internalSyncronized;

		//function associate to syncronisation
		void clearPool();
	};

	//!\file Pool.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class VertexPool
	//!\brief Handle Vertices for the GeometryManager
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

		// Vertices handler
		Key<Element<Vertices>> addVertices(Vertices const &vertices);
		VertexPool &rmVertices(Key<Element<Vertices>> const &key);
		VertexPool &clearPool();

		// tool for link index pool and  vertex pool (usefull for draw with indices ;p)
		VertexPool &attachIndexPoolToVertexPool(IndexPool const &pool);
		VertexPool &dettachIndexPoolToVertexPool();

		//draw and synchronisation
		virtual Pool &syncronisation();
		virtual Buffer const &getBuffer() const;
		VertexPool const &draw(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt) const;
		VertexPool const &draw(GLenum mode, Key<Element<Vertices>> const &drawIt) const;

	private:
		VertexBuffer _vbo;
		VertexArray _vao;
		IndexPool const *_indexPoolattach;

		// reprensent data in vbo
		std::map<Key<Element<Vertices>>, Element<Vertices>> _poolElement;

	};

	//!\file Pool.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class IndexPool
	//!\brief Handle Indices for the GeometryManager
	class IndexPool : public Pool
	{
	public:
		IndexPool();
		IndexPool(IndexPool const &copy);
		virtual ~IndexPool();
		virtual IndexPool &operator=(IndexPool const &p);
		
		virtual Pool &syncronisation();
		virtual Buffer const &getBuffer() const;

		// Vertices handler
		Key<Element<Indices>> addIndices(Indices const &indices);
		IndexPool &rmIndices(Key<Element<Indices>> const &key);
		IndexPool &clearPool();

		// tool for link index pool and  vertex pool (usefull for draw with indices ;p)
		IndexPool &attachVertexPoolToIndexPool(VertexPool const &pool);
		IndexPool &dettachVertexPoolToIndexPool();

	private:
		gl::IndexBuffer _ibo;
		VertexPool const *_vertexPoolattach;

		// reprsent data in ibo
		std::map<Key<Element<Indices>>, Element<Indices>> _poolElement;

		// Warning must be call only by VertexPool
		IndexPool const &onDrawCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target) const;
		friend VertexPool const &VertexPool::draw(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt) const;
	};
}