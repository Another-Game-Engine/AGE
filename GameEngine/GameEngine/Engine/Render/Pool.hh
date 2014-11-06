#pragma once

#include <Utils/OpenGL.hh>
#include <Render/Key.hh>
#include <Render/VertexArray.hh>
#include <Render/Buffer.hh>
#include <unordered_map>
#include <Render/MemoryGPU.hh>
#include <stdint.h>
#include <Utils/Containers/Vector.hpp>
#include <cassert>
#include <Render/Data.hh>

class VertexPool;
class IndexPool;
class Vertices;
class Indices;
class MemoryBlocksGPU;

#define KEY_ELEMENT_POOL 0

struct ElementPool
{
	size_t memoryIndex;
	Data data;
	ElementPool() 
		: memoryIndex(0) {}

	ElementPool(size_t memoryIndex, Data const &d) : memoryIndex(memoryIndex), data(d) {}
	ElementPool(ElementPool const &copy) : memoryIndex(copy.memoryIndex), data(copy.data){}
	ElementPool &operator=(ElementPool const &e) { memoryIndex = e.memoryIndex; data = e.data; return (*this); }
	~ElementPool(){}
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
	Key<DATA> addElementPool(Data const &vertices);
	ElementPool *getElementPool(Key<DATA> const &key){ assert(!!key); return (&_poolElement[key.getId()]); }
	Pool<POOL, DATA, BUFFER> &rmElement(Key<DATA> &key);
	Pool<POOL, DATA, BUFFER> &syncronisation();
	Pool<POOL, DATA, BUFFER> &bind();

protected:
	BUFFER _buffer;
	AGE::Vector<AttributeData> _attributes;
	size_t _nbrBytePool;
	AGE::Vector<ElementPool> _poolElement;
	AGE::Vector<MemoryBlocksGPU> _poolMemory;
	bool _setContext;
	bool _reloadBuffer;
	bool _internalSyncronized;
	size_t _poolNumber;

private:
	bool checkMemoryStorage(Key<DATA> const &key, Data const &element);
	void resetSyncronisation();
	void buildNewMemoryStorage(Data const &element);
	void internalSycronisation();
};

template <typename POOL, typename DATA, typename BUFFER>
Pool<POOL, DATA, BUFFER>::Pool()
	: _nbrBytePool(0),
	_setContext(true),
	_reloadBuffer(false),
	_internalSyncronized(false)
{
	static size_t id = 0;
	_poolNumber = id++;
}

template <typename POOL, typename DATA, typename BUFFER>
Pool<POOL, DATA, BUFFER>::Pool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &type, AGE::Vector<uint8_t> const &sizetype, AGE::Vector<uint8_t> const &nbrComponent)
	: Pool<POOL, DATA, BUFFER>()
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
	_internalSyncronized(copy._internalSyncronized),
	_poolNumber(copy._poolNumber)
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
	_poolNumber = p._poolNumber;
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
bool Pool<POOL, DATA, BUFFER>::checkMemoryStorage(Key<DATA> const &key, Data const &element)
{
	for (size_t index = 0; index < _poolMemory.size(); ++index)
	{
		MemoryBlocksGPU &memory = _poolMemory[index];
		if (!memory.getIsUsed() && memory.getNbrElement() == element.getNbrElement())
		{
			_internalSyncronized = false;
			memory.setSync(false); memory.setIsUsed(true);
			_poolElement[key.getId()] = ElementPool(index, element);
			return (true);
		}
	}
	return (false);
}

template <typename POOL, typename DATA, typename BUFFER>
void Pool<POOL, DATA, BUFFER>::buildNewMemoryStorage(Data const &element)
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
Key<DATA> Pool<POOL, DATA, BUFFER>::addElementPool(Data const &element)
{
	Key<DATA> key;
	if (checkMemoryStorage(key, element))
		return (key);
	key = Key<DATA>::createKey(_poolNumber);
	resetSyncronisation();
	buildNewMemoryStorage(element);
	if (_poolElement.size() >= key.getId())
		_poolElement.push_back(ElementPool());
	_poolElement[key.getId()] = ElementPool(_poolMemory.size() - 1, element);
	return (key);
}

template <typename POOL, typename DATA, typename BUFFER>
Pool<POOL, DATA, BUFFER> &Pool<POOL, DATA, BUFFER>::rmElement(Key<DATA> &key)
{
	ElementPool *element = getElementPool(key);
	MemoryBlocksGPU &memory = _poolMemory[element->memoryIndex];
	memory.setSync(true);
	memory.setIsUsed(false);
	key.destroy();
	return (*this);
}

template <typename POOL, typename DATA, typename BUFFER>
void Pool<POOL, DATA, BUFFER>::internalSycronisation()
{
	for (auto &element : _poolElement)
	{
		MemoryBlocksGPU &memory = _poolMemory[element.memoryIndex];
		if (!memory.getSync())
		{
			memory.setSync(true);
			for (size_t index = 0; index < memory.getNbrBlock(); ++index)
			{
				_buffer.BufferSubData(_attributes[index].offsetAttribute + memory.getOffset(uint8_t(index)), memory.getSizeBlock(uint8_t(index)), (void *)element.data.getBuffer(uint8_t(index)));
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
	VertexPool &draw(GLenum mode, Key<Vertices> const &key);
	VertexPool &draw(GLenum mode, Key<Indices> const &i, Key<Vertices> const &v);

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
	IndexPool &draw(GLenum mode, Key<Indices> const &i, size_t start);
};
