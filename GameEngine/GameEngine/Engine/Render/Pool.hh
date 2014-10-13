#pragma once

#include <Utils/OpenGL.hh>
#include <Render/Key.hh>
#include <Render/VertexArray.hh>
#include <Render/Buffer.hh>
#include <map>
#include <Render/MemoryGPU.hh>
#include <stdint.h>
#include <Render/Data.hh>
#include <Utils/Containers/Vector.hpp>
#include <cassert>

namespace gl
{
	class VertexPool;
	class IndexPool;
	class Vertices;
	class Indices;
	class MemoryBlocksGPU;

	template <typename TYPE>
	struct Element
	{
		size_t memoryIndex;
		TYPE const *data;
		Element(size_t memoryIndex, TYPE const *data) : memoryIndex(memoryIndex), data(data) {}
		Element(Element const &copy) : memoryIndex(copy.memoryIndex), data(copy.data){}
		Element &operator=(Element const &e) { memoryIndex = e.memoryIndex; data = e.data; return (*this); }
		~Element();
	};

	struct AttributeData
	{
		GLenum typeComponent;
		uint8_t sizeTypeComponent;
		uint8_t nbrComponent;
		size_t sizeAttribute;
		size_t offsetAttribute;
		AttributeData();
		AttributeData(GLenum tc, uint8_t stc, uint8_t nbrC);
		~AttributeData();
		AttributeData(AttributeData const &copy);
		AttributeData &operator=(AttributeData const &a);
	};

	template <typename POOL, typename DATA>
	class Pool
	{
	public:
		Pool();
		Pool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &type, AGE::Vector<uint8_t> const &sizetype, AGE::Vector<uint8_t> const &nbrComponent)
		virtual ~Pool();

	public:
		uint8_t getNbrAttribute() const { return (_data.size()); }
		GLenum getTypeComponent(uint8_t index) { return (_data[index].typeComponent); }
		uint8_t getSizeTypeComponent(uint8_t index) { return (_data[index].sizeTypeComponent); }
		uint8_t getNbrComponent(uint8_t index) const { return (_data[index].nbrComponent); }
		size_t getSizeAttribute(uint8_t index) const { return (_data[index].sizeAttribute); }
		size_t getOffsetAttribute(uint8_t index) const { return (_data[index].offsetAttribute); }
		size_t getNbrBytePool() const { return (_nbrBytePool); }
	public:
		virtual Pool &syncronisation() = 0;
		virtual Buffer const &getBuffer() const = 0;
		Key<Element<DATA>> addElementPool(Element<DATA> &poolElement, DATA const &vertices);
		Element<DATA> *getElementPool(Key<Element<DATA>> const &key);
		POOL &rmElement(Key<Element<DATA>> &key);

	protected:
		AGE::Vector<AttributeData> _attributes;
		size_t _nbrBytePool;
		std::map<Key<Element<DATA>>, Element<DATA>> _poolElement;
		AGE::Vector<MemoryBlocksGPU> _poolMemory;
		bool _syncronized;
		bool _internalSyncronized;
		std::pair<Key<Element<DATA>>, Element<DATA> *>> _optimizerElementSearch;

	private:
		Pool(Pool const &copy) = delete;
		Pool &operator=(Pool const &p) = delete;
		bool checkMemoryStorage(Key<Element<DATA>> const &key, DATA const &element);
		void resetSyncronisation();
		void buildNewMemoryStorage(DATA const &element);
	};

	template <typename POOL, typename DATA>
	Pool<POOL, DATA>::Pool()
		: _nbrBytePool(0),
		_syncronized(true),
		_internalSyncronized(true)
	{
	}

	template <typename POOL, typename DATA>
	Pool<POOL, DATA>::Pool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &type, AGE::Vector<uint8_t> const &sizetype, AGE::Vector<uint8_t> const &nbrComponent)
		: _nbrBytePool(0),
		_syncronized(true),
		_internalSyncronized(true)
	{
		_attributes.resize(nbrAttribute);
		for (auto &attribute : _attributes)
		{
			attribute.typeComponent = type;
			attribute.sizeTypeComponent = sizetype;
			attribute.nbrComponent = nbrComponent[index];
		}
	}

	template <typename POOL, typename DATA>
	Pool<POOL, DATA>::~Pool()
	{
	}

	template <typename POOL, typename DATA>
	void Pool<POOL, DATA>::clearPool()
	{
		_syncronized = true;
		_internalSyncronized = true;
		_nbrBytePool = 0;
		_poolMemory.clear();
	}

	template <typename POOL, typename DATA>
	void Pool<POOL, DATA>::resetSyncronisation()
	{
		_syncronized = false;
		_internalSyncronized = false;
		for (int index = 0; index < _poolMemory.size(); ++index)
			_poolMemory[index].setSync(false);
	}
	
	template <typename POOL, typename DATA>
	bool Pool<POOL, DATA>::checkMemoryStorage(Key<Element<DATA>> const &key, DATA const &element)
	{
		for (size_t index = 0; index < _poolMemory.size(); ++index)
		{
			MemoryBlocksGPU &memory = _poolMemory[index];
			if (!memory.getIsUsed() && memory.getNbrElement() == element.getNbrElement())
			{
				_internalSyncronized = false;
				memory.setSync(false); memory.setIsUsed(true);
				_poolElement[key] = Element<DATA>(index, &element);
				return (true);
			}
		}
		return (false);
	}

	template <typename POOL, typename DATA>
	void Pool<POOL, DATA>::buildNewMemoryStorage(DATA const &element)
	{
		MemoryBlocksGPU memory;
		memory.setNbrElement(element.getNbrElement()); 
		memory.setNbrBlock(_attributes.size());
		for (uint8_t index = 0; index < memory.getNbrBlocks(); ++index)
		{
			size_t sizeAttribute = _attributes[index].sizeTypeComponent;
			sizeAttribute *= _attributes[index].nbrComponent;
			sizeAttribute *= vertices.getNbrElement();
			_nbrBytePool += sizeAttribute;
			_attributes[index].sizeAttribute += sizeAttribute;
			memory.setSizeBlock(index, sizeAttribute);
			if (index > 0)
			{
				_attributes[index].offsetAttribute = _attributes[index - 1].offsetAttribute;
				_attributes[index].offsetAttribute += _attributes[index - 1].sizeAttribute;
			}
		}
		_poolMemory.push_back(memory);
	}

	template <typename POOL, typename DATA>
	Key<Element<DATA>> Pool<POOL, DATA>::addElementPool(Element<DATA> &poolElement, DATA const &element)
	{
		Key<Element<DATA>> key = Key<Element<DATA>>::createKey();
		if (checkMemoryStorage(key, element))
			return (key);
		resetSyncronisation();
		buildNewMemoryStorage(element);
		poolElement[key] = Element<DATA>(_poolMemory.size() - 1, &element);
		return (key);
	}

	template <typename POOL, typename DATA>
	Element<DATA> *Pool<POOL, DATA>::getElementPool(Key<Element<DATA>> const &key)
	{
		if (!key)
			assert(0);
		if (_poolElement.size() == 0)
			assert(0);
		if (_optimizerElementSearch.first == key)
			return (_optimizerElementSearch.second);
		auto &it = _poolElement.find(key);
		if (it == _poolElement.end())
			assert(0);
		_optimizerElementSearch.first = key;
		_optimizerElementSearch.second = &it->second;
		return (&it->second);
	}

	template <typename POOL, typename DATA>
	POOL &Pool<POOL, DATA>::rmElement(Key<Element<DATA>> &key)
	{
		Element<DATA> const *element = getElementPool(key)
		MemoryBlocksGPU &memory = _poolMemory[element->memoryIndex];
		memory.setSync(true);
		memory.setIsUsed(false);
		_poolElement.erase(key);
		key.destroy();
		return (*this);
	}

	class VertexPool : public Pool<VertexPool, Vertices>
	{
	public:
		VertexPool();
		VertexPool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t> const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent);
		virtual ~VertexPool();
		VertexPool &attachIndexPoolToVertexPool(IndexPool const &pool);
		VertexPool &dettachIndexPoolToVertexPool();
		virtual Pool &syncronisation();
		virtual Buffer const &getBuffer() const;
		VertexPool const &draw(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt) const;
		VertexPool const &draw(GLenum mode, Key<Element<Vertices>> const &drawIt) const;
		VertexPool const &drawInstanced(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt, size_t nbrIntanced) const;
	
	private:
		VertexPool(VertexPool const &copy) = delete;
		VertexPool &operator=(VertexPool const &pool) = delete;
	
	private:
		VertexBuffer _vbo;
		VertexArray _vao;
		IndexPool const *_indexPoolattach;
	};

	class IndexPool : public Pool<IndexPool, Indices>
	{
	public:
		IndexPool();
		virtual ~IndexPool();
		virtual Pool &syncronisation();
		virtual Buffer const &getBuffer() const;
		IndexPool &attachVertexPoolToIndexPool(VertexPool const &pool);
		IndexPool &dettachVertexPoolToIndexPool();
		IndexPool const &onDrawCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target) const;
		IndexPool const &onDrawIntancedCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target, size_t nbrInstanced) const;
	
	private:
		IndexBuffer _ibo;
		VertexPool const *_vertexPoolattach;
	
	private:
		IndexPool(IndexPool const &copy) = delete;
		IndexPool &operator=(IndexPool const &p) = delete;

	};
}