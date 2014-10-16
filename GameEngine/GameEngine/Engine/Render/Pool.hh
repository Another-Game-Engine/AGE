#pragma once

#include <Utils/OpenGL.hh>
#include <Render/Key.hh>
#include <Render/VertexArray.hh>
#include <Render/Buffer.hh>
#include <unordered_map>
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

#define KEY_ELEMENT_POOL 0;

	template <typename TYPE>
	struct Element
	{
		size_t memoryIndex;
		TYPE const *data;
		Element() : memoryIndex(0), data(nullptr) {}
		Element(size_t memoryIndex, TYPE const *data) : memoryIndex(memoryIndex), data(data) {}
		Element(Element const &copy) : memoryIndex(copy.memoryIndex), data(copy.data){}
		Element &operator=(Element const &e) { memoryIndex = e.memoryIndex; data = e.data; return (*this); }
		~Element(){}
		void reset() { data = nullptr; memoryIndex = 0; }
		bool operator!() { return data != nullptr; }
	};

	template <typename TYPE> 
	struct HashKey {};

	template <>
	struct HashKey<Key<Element<Indices>>>
	{
		std::size_t operator()(Key<Element<Indices>> const &k) const
		{
			return (std::hash<std::size_t>()(k.getId()));
		}
	};

	template <>
	struct HashKey<Key<Element<Vertices>>>
	{
		std::size_t operator()(Key<Element<Vertices>> const &k) const
		{
			return (std::hash<std::size_t>()(k.getId()));
		}
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

	template <typename POOL, typename DATA, typename BUFFER>
	class Pool
	{
	public:
		Pool();
		Pool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &type, AGE::Vector<uint8_t> const &sizetype, AGE::Vector<uint8_t> const &nbrComponent);
		Pool(Pool const &copy);
		Pool<POOL, DATA, BUFFER> &operator=(Pool const &p); 
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
		Key<Element<DATA>> addElementPool(DATA const &vertices);
		Element<DATA> *getElementPool(Key<Element<DATA>> const &key);
		Pool<POOL, DATA, BUFFER> &rmElement(Key<Element<DATA>> &key);
		Pool<POOL, DATA, BUFFER> &syncronisation();
		Pool<POOL, DATA, BUFFER> &bind();

	protected:
		BUFFER _buffer;
		AGE::Vector<AttributeData> _attributes;
		size_t _nbrBytePool;
		AGE::Vector<Element<DATA>> _poolElement;
		AGE::Vector<MemoryBlocksGPU> _poolMemory;
		bool _setContext;
		bool _reloadBuffer;
		bool _internalSyncronized;
		std::pair<Key<Element<DATA>>, Element<DATA> *> _optimizerElementSearch;

	private:
		bool checkMemoryStorage(Key<Element<DATA>> const &key, DATA const &element);
		void resetSyncronisation();
		void buildNewMemoryStorage(DATA const &element);
		void internalSycronisation();
	};

	template <typename POOL, typename DATA, typename BUFFER>
	Pool<POOL, DATA, BUFFER>::Pool()
		: _nbrBytePool(0),
		_setContext(true),
		_reloadBuffer(false),
		_internalSyncronized(false)
	{
	}

	template <typename POOL, typename DATA, typename BUFFER>
	Pool<POOL, DATA, BUFFER>::Pool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &type, AGE::Vector<uint8_t> const &sizetype, AGE::Vector<uint8_t> const &nbrComponent)
		: _nbrBytePool(0),
		_setContext(true),
		_reloadBuffer(false),
		_internalSyncronized(false)
	{
		_attributes.resize(nbrAttribute);
		for (size_t index = 0; index < _attributes.size(); ++index)
		{
			auto &attribute = _attributes[index];
			attribute.typeComponent = type[index];
			attribute.sizeTypeComponent = sizetype[index];
			attribute.nbrComponent = nbrComponent[index];
		}
	}

	template <typename POOL, typename DATA, typename BUFFER>
	Pool<POOL, DATA, BUFFER>::Pool(Pool<POOL, DATA, BUFFER> const &copy)
		: _attributes(copy._attributes),
		_nbrBytePool(copy._nbrBytePool),
		_setContext(copy._setContext),
		_reloadBuffer(copy._reloadBuffer),
		_internalSyncronized(copy._internalSyncronized)
	{

	}

	template <typename POOL, typename DATA, typename BUFFER>
	Pool<POOL, DATA, BUFFER> &Pool<POOL, DATA, BUFFER>::operator=(Pool<POOL, DATA, BUFFER> const &p)
	{
		_attributes = p._attributes;
		_nbrBytePool = p._nbrBytePool;
		_setContext = p._setContext;
		_reloadBuffer = p._reloadBuffer;
		_internalSyncronized = p._internalSyncronized;
		return (*this);
	}

	template <typename POOL, typename DATA, typename BUFFER>
	Pool<POOL, DATA, BUFFER>::~Pool()
	{
	}

	template <typename POOL, typename DATA, typename BUFFER>
	void Pool<POOL, DATA, BUFFER>::resetSyncronisation()
	{
		_setContext = false;
		_reloadBuffer = false;
		_internalSyncronized = false;
		for (int index = 0; index < _poolMemory.size(); ++index)
			_poolMemory[index].setSync(false);
	}
	
	template <typename POOL, typename DATA, typename BUFFER>
	bool Pool<POOL, DATA, BUFFER>::checkMemoryStorage(Key<Element<DATA>> const &key, DATA const &element)
	{
		for (size_t index = 0; index < _poolMemory.size(); ++index)
		{
			MemoryBlocksGPU &memory = _poolMemory[index];
			if (!memory.getIsUsed() && memory.getNbrElement() == element.getNbrElement())
			{
				_internalSyncronized = false;
				memory.setSync(false); memory.setIsUsed(true);
				_poolElement[key.getId()] = Element<DATA>(index, &element);
				return (true);
			}
		}
		return (false);
	}

	template <typename POOL, typename DATA, typename BUFFER>
	void Pool<POOL, DATA, BUFFER>::buildNewMemoryStorage(DATA const &element)
	{
		MemoryBlocksGPU memory;
		memory.setNbrElement(element.getNbrElement()); 
		memory.setNbrBlock(_attributes.size());
		for (uint8_t index = 0; index < memory.getNbrBlock(); ++index)
		{
			memory.setSizeBlock(index, _attributes[index].sizeTypeComponent * _attributes[index].nbrComponent * element.getNbrElement());
			memory.setOffset(index, _attributes[index].sizeAttribute);
			_nbrBytePool += memory.getSizeBlock(index);
			_attributes[index].sizeAttribute += memory.getSizeBlock(index);
			if (index > 0)
			{
				_attributes[index].offsetAttribute = _attributes[index - 1].offsetAttribute + _attributes[index - 1].sizeAttribute;
			}
		}
		if (_poolMemory.size() > 0)
		{
			memory.setStartElement(_poolMemory.back().getElementStart() + _poolMemory.back().getNbrElement());
		}
		else
		{
			memory.setStartElement(0);
		}
		_poolMemory.push_back(memory);
	}

	template <typename POOL, typename DATA, typename BUFFER>
	Key<Element<DATA>> Pool<POOL, DATA, BUFFER>::addElementPool(DATA const &element)
	{
		Key<Element<DATA>> key = Key<Element<DATA>>::createKey();
		if (checkMemoryStorage(key, element))
			return (key);
		resetSyncronisation();
		buildNewMemoryStorage(element);
		if (_poolElement.size() < key.getId())
			_poolElement.resize(key.getId() + 1);
		_poolElement[key.getId()] = Element<DATA>(_poolMemory.size() - 1, &element);
		return (key);
	}

	template <typename POOL, typename DATA, typename BUFFER>
	Element<DATA> *Pool<POOL, DATA, BUFFER>::getElementPool(Key<Element<DATA>> const &key)
	{
		if (!key)
			assert(0);
		if (_poolElement.size() == 0 || key.getId() >= _poolElement.size())
			assert(0);
		if (_optimizerElementSearch.first == key)
			return (_optimizerElementSearch.second);
		auto &res = _poolElement[key.getId()];
		_optimizerElementSearch.second = &res;
		_optimizerElementSearch.first = key;
		return (&res);
	}

	template <typename POOL, typename DATA, typename BUFFER>
	Pool<POOL, DATA, BUFFER> &Pool<POOL, DATA, BUFFER>::rmElement(Key<Element<DATA>> &key)
	{
		Element<DATA> *element = getElementPool(key);
		MemoryBlocksGPU &memory = _poolMemory[element->memoryIndex];
		memory.setSync(true);
		memory.setIsUsed(false);
		element->reset();
		key.destroy();
		return (*this);
	}

	template <typename POOL, typename DATA, typename BUFFER>
	void Pool<POOL, DATA, BUFFER>::internalSycronisation()
	{
		for (auto &element : _poolElement)
		{
			if (element.data == nullptr)
				continue;
			MemoryBlocksGPU &memory = _poolMemory[element.memoryIndex];
			if (!memory.getSync())
			{
				memory.setSync(true);
				for (size_t index = 0; index < memory.getNbrBlock(); ++index)
				{
					_buffer.BufferSubData(_attributes[index].offsetAttribute + memory.getOffset(uint8_t(index)), memory.getSizeBlock(uint8_t(index)), (void *)element.data->getBuffer(uint8_t(index)));
				}
			}
		}
	}

	template <typename POOL, typename DATA, typename BUFFER>
	Pool<POOL, DATA, BUFFER> &Pool<POOL, DATA, BUFFER>::bind()
	{
		_buffer.bind();
	}

	template <typename POOL, typename DATA, typename BUFFER>
	Pool<POOL, DATA, BUFFER> &Pool<POOL, DATA, BUFFER>::syncronisation()
	{
		if (_reloadBuffer && _internalSyncronized)
		{
			return (*this);
		}
		_buffer.bind();
		if (!_reloadBuffer)
		{
			_buffer.BufferData(_nbrBytePool);
			_reloadBuffer = true;
		}
		if (!_internalSyncronized)
		{
			internalSycronisation();
			_internalSyncronized = true;
		}
		return (*this);
	}

	class VertexPool : public Pool<VertexPool, Vertices, VertexBuffer>
	{
	public:
		VertexPool();
		VertexPool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t> const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent);
		virtual ~VertexPool();
		VertexPool(VertexPool const &copy);
		VertexPool &operator=(VertexPool const &pool); 
		VertexPool const &bind() const;
		VertexPool &startContext();
		VertexPool &startContext(IndexPool &i);
		VertexPool const &endContext() const;
		VertexPool &draw(GLenum mode, Key<Element<Vertices>> const &key);
		VertexPool &draw(GLenum mode, Key<Element<Indices>> const &i, Key<Element<Vertices>> const &v);

	private:
		VertexArray _context;
		IndexPool *_indexPool;
		bool _contextSet;
	};

	class IndexPool : public Pool<IndexPool, Indices, IndexBuffer>
	{
	public:
		IndexPool();
		virtual ~IndexPool();
		IndexPool const &bind() const;
		IndexPool(IndexPool const &copy);
		IndexPool &operator=(IndexPool const &p);
		IndexPool &draw(GLenum mode, Key<Element<Indices>> const &i, size_t start);
	};
}