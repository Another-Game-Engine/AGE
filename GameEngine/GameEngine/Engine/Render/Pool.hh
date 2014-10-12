#pragma once

#include <Utils/OpenGL.hh>
#include <Render/Key.hh>
#include <Render/VertexArray.hh>
#include <Render/Buffer.hh>
#include <map>
#include <Utils/Containers/Vector.hpp>
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
		AGE::Vector<MemoryBlocksGPU> _poolMemory;
		size_t _nbrBytePool;
		size_t _nbrElementPool;
		bool _syncronized;
		bool _internalSyncronized;

		//function associate to syncronisation
		void clearPool();

		// tool use in intern
		void clear();
		template <typename TYPE> Key<Element<TYPE>> addElementPool(std::map<Key<Pool::Element<TYPE>>, Element<TYPE>> &poolElement, TYPE const &vertices);
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
		VertexPool &rmVertices(Key<Element<Vertices>> &key);
		VertexPool &clearPool();

		// tool for link index pool and  vertex pool (usefull for draw with indices ;p)
		VertexPool &attachIndexPoolToVertexPool(IndexPool const &pool);
		VertexPool &dettachIndexPoolToVertexPool();

		//draw and synchronisation
		virtual Pool &syncronisation();
		virtual Buffer const &getBuffer() const;
		VertexPool const &draw(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt) const;
		VertexPool const &draw(GLenum mode, Key<Element<Vertices>> const &drawIt) const;
		VertexPool const &drawInstanced(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt, size_t nbrIntanced) const;

	private:
		VertexBuffer _vbo;
		VertexArray _vao;
		IndexPool const *_indexPoolattach;

		// reprensent data in vbo
		std::map<Key<Element<Vertices>>, Element<Vertices>> _poolElement;
		// tool use in intern
		Element<Vertices> const *getVerticesPoolElement(Key<Element<Vertices>> const &key, std::string const &msg) const;
		
		friend IndexPool;
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
		IndexPool &rmIndices(Key<Element<Indices>> &key);
		IndexPool &clearPool();

		// tool for link index pool and  vertex pool (usefull for draw with indices ;p)
		IndexPool &attachVertexPoolToIndexPool(VertexPool const &pool);
		IndexPool &dettachVertexPoolToIndexPool();
		IndexPool const &onDrawCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target) const;
		IndexPool const &onDrawIntancedCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target, size_t nbrInstanced) const;

	private:
		gl::IndexBuffer _ibo;
		VertexPool const *_vertexPoolattach;
		// reprsent data in ibo
		std::map<Key<Element<Indices>>, Element<Indices>> _poolElement;
		// tool use in intern
		Element<Indices> const *getIndicesPoolElement(Key<Element<Indices>> const &key, std::string const &msg) const;
		
	};

	template <typename TYPE>
	Key<Pool::Element<TYPE>> Pool::addElementPool(std::map<Key<Pool::Element<TYPE>>, Element<TYPE>> &poolElement, TYPE const &vertices)
	{
		for (size_t index = 0; index < _poolMemory.size(); ++index)
		{
			MemoryBlocksGPU &memory = _poolMemory[index];
			if (_poolMemory[index].getIsUsed() == false && memory.getNbrElement() == vertices.getNbrElement())
			{
				_internalSyncronized = false;
				memory.setSync(false);
				memory.setIsUsed(true);
				Key<Pool::Element<TYPE>> keyElement = Key<Pool::Element<TYPE>>::createKey();
				Pool::Element<TYPE> newElement;
				newElement.memoryIndex = index;
				newElement.data = &vertices;
				poolElement[keyElement] = newElement;
				return (keyElement);
			}
		}
		_syncronized = false; // cause reset of buffer on gpu
		_internalSyncronized = false;
		for (int index = 0; index < _poolMemory.size(); ++index)
			_poolMemory[index].setSync(false);
		MemoryBlocksGPU memory;
		memory.setNbrElement(vertices.getNbrElement());
		memory.setNbrBlock(_nbrAttribute);
		for (uint8_t index = 0; index < _nbrAttribute; ++index)
		{
			size_t sizeAttribute = _sizeTypeComponent[index] * _nbrComponent[index] * vertices.getNbrElement();
			_nbrBytePool += sizeAttribute;
			_sizeAttribute[index] += sizeAttribute;
			memory.setSizeBlock(index, sizeAttribute);
			if (index > 0)
				_offsetAttribute[index] = _offsetAttribute[index - 1] + _sizeAttribute[index - 1];
		}
		_poolMemory.push_back(memory);
		Element<TYPE> element;
		Key<Element<TYPE>> keyElement = Key<Element<TYPE>>::createKey();
		element.data = &vertices;
		element.memoryIndex = _poolMemory.size() - 1;
		poolElement[keyElement] = element;
		return (keyElement);
	}
}