#include <Render/Pool.hh>
#include <Render/MemoryGPU.hh>
#include <Render/Data.hh>
#include <iostream>
#include <string>
#include <cassert>

#define INDEXPOOL 1
#define DEBUG_MESSAGE(type, from, key_word, reason, end) \
	{ assert(0 && std::string(std::string(type) + ":from[" + std::string(from) + "].key-word[" + std::string(key_word) + "].reason[" + std::string(reason) + "]").c_str()); return end; }

namespace gl
{

	Pool::Pool()
		: _nbrAttribute(4),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_sizeAttribute(NULL),
		_offsetAttribute(NULL),
		_nbrBytePool(0),
		_nbrElementPool(0),
		_syncronized(true),
		_internalSyncronized(true)
	{
		_typeComponent = new GLenum[_nbrAttribute];
		_sizeTypeComponent = new uint8_t[_nbrAttribute];
		_nbrComponent = new uint8_t[_nbrAttribute];
		_sizeAttribute = new size_t[_nbrAttribute];
		_offsetAttribute = new size_t[_nbrAttribute];
		memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
		memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		for (uint8_t index = 0; index < _nbrAttribute - 1; ++index)
		{
			_typeComponent[index] = GL_FLOAT;
			_sizeTypeComponent[index] = sizeof(float);
			_nbrComponent[index] = 4;
		}
		_typeComponent[_nbrAttribute - 1] = GL_FLOAT;
		_sizeTypeComponent[_nbrAttribute - 1] = sizeof(float);
		_nbrComponent[_nbrAttribute - 1] = 2;
	}

	Pool::Pool(int)
		: _nbrAttribute(1),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_sizeAttribute(NULL),
		_offsetAttribute(NULL),
		_nbrBytePool(0),
		_nbrElementPool(0),
		_syncronized(true),
		_internalSyncronized(true)
	{
		_typeComponent = new GLenum;
		_sizeTypeComponent = new uint8_t;
		_nbrComponent = new uint8_t;
		_sizeAttribute = new size_t;
		_offsetAttribute = new size_t;
		*_typeComponent = GL_UNSIGNED_INT;
		*_sizeTypeComponent = sizeof(unsigned int);
		*_nbrComponent = 1;
		*_sizeAttribute = 0;
		*_offsetAttribute = 0;
	}

	Pool::Pool(uint8_t nbrAttribute, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
		: _nbrAttribute(nbrAttribute),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_sizeAttribute(NULL),
		_offsetAttribute(NULL),
		_nbrBytePool(0),
		_nbrElementPool(0),
		_syncronized(true),
		_internalSyncronized(true)
	{
		if (_nbrAttribute)
		{
			_typeComponent = new GLenum[_nbrAttribute];
			memcpy(_typeComponent, typeComponent, sizeof(GLenum)* _nbrAttribute);
			_sizeTypeComponent = new uint8_t[_nbrAttribute];
			memcpy(_sizeTypeComponent, sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			_nbrComponent = new uint8_t[_nbrAttribute];
			memcpy(_nbrComponent, nbrComponent, sizeof(uint8_t)* _nbrAttribute);
			_sizeAttribute = new size_t[_nbrAttribute];
			memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			_offsetAttribute = new size_t[_nbrAttribute];
			memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		}
	}

	Pool::Pool(Pool const &copy)
		: _nbrAttribute(copy._nbrAttribute),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_sizeAttribute(NULL),
		_offsetAttribute(NULL),
		_nbrBytePool(0),
		_nbrElementPool(0),
		_syncronized(true)
	{
		if (_nbrAttribute)
		{
			_typeComponent = new GLenum[_nbrAttribute];
			memcpy(_typeComponent, copy._typeComponent, sizeof(GLenum)* _nbrAttribute);
			_sizeTypeComponent = new uint8_t[_nbrAttribute];
			memcpy(_sizeTypeComponent, copy._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			_nbrComponent = new uint8_t[_nbrAttribute];
			memcpy(_nbrComponent, copy._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
			_sizeAttribute = new size_t[_nbrAttribute];
			memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			_offsetAttribute = new size_t[_nbrAttribute];
			memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		}
	}

	Pool::~Pool()
	{
		clear();
	}

	void Pool::clear()
	{
		if (_nbrAttribute)
		{
			delete[] _typeComponent;
			delete[] _sizeTypeComponent;
			delete[] _nbrComponent;
			delete[] _sizeAttribute;
			delete[] _offsetAttribute;
		}
	}

	// set all data with "p" data
	Pool &Pool::operator=(Pool const &p)
	{
		if (this != &p)
		{
			if (_nbrAttribute != p._nbrAttribute)
			{
				clear();
				_nbrAttribute = p._nbrAttribute;
				if (_nbrAttribute)
				{
					_typeComponent = new GLenum[p._nbrAttribute];
					_sizeTypeComponent = new uint8_t[p._nbrAttribute];
					_nbrComponent = new uint8_t[p._nbrAttribute];
					_sizeAttribute = new size_t[p._nbrAttribute];
					_offsetAttribute = new size_t[p._nbrAttribute];
				}
				else
				{
					_typeComponent = NULL;
					_sizeTypeComponent = NULL;
					_nbrComponent = NULL;
					_sizeAttribute = NULL;
					_offsetAttribute = NULL;
				}
			}
			memcpy(_typeComponent, p._typeComponent, sizeof(GLenum)* _nbrAttribute);
			memcpy(_sizeTypeComponent, p._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			memcpy(_nbrComponent, p._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
			memset(_sizeAttribute, 0, sizeof(size_t) * _nbrAttribute);
			memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		}
		return (*this);
	}

	uint8_t Pool::getNbrAttribute() const
	{
		return (_nbrAttribute);
	}

	GLenum Pool::getTypeComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getTypeComponent", "Index out of range", -1)
		return (_typeComponent[index]);
	}

	uint8_t Pool::getSizeTypeComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getSizeTypeComponent", "Index out of range", -1)
		return (_sizeTypeComponent[index]);
	}

	uint8_t Pool::getNbrComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getNbrComponent", "Index out of range", -1)
		return (_nbrComponent[index]);
	}

	size_t Pool::getSizeAttribute(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getSizeAttribute", "Index out of range", -1)
			return (_sizeAttribute[index]);
	}

	size_t Pool::getOffsetAttribute(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getOffsetAttribute", "Index out of range", -1)
			return (_offsetAttribute[index]);
	}


	size_t Pool::getNbrBytePool() const
	{
		return (_nbrBytePool);
	}

	Key<Pool::Element<Vertices>> VertexPool::addVertices(Vertices const &vertices)
	{
		// Check in memory pool if a field is free
		for (size_t index = 0; index < _poolMemory.size(); ++index)
		{
			MemoryBlocksGPU &memory = _poolMemory[index];
			if (_poolMemory[index].getIsUsed() == false && memory.getNbrElement() == vertices.getNbrVertices())
			{
				_internalSyncronized = false;
				memory.setSync(false);
				memory.setIsUsed(true);
				Key<Element<Vertices>> keyElement;
				Element<Vertices> newElement;
				newElement.memoryIndex = index;
				newElement.data = &vertices;
				_poolElement[keyElement] = newElement;
				return (keyElement);
			}
		}
		// Udpate major is require cause of size pool changement
		_syncronized = false; // cause reset of buffer on gpu
		_internalSyncronized = false;
		for (int index = 0; index < _poolMemory.size(); ++index)
			_poolMemory[index].setSync(false); // ask an update of all element of memory gpu
		MemoryBlocksGPU memory;
		memory.setNbrElement(vertices.getNbrVertices());
		memory.setNbrBlock(_nbrAttribute);
		for (uint8_t index = 0; index < _nbrAttribute; ++index)
		{
			size_t sizeAttribute = _sizeTypeComponent[index] * _nbrComponent[index] * vertices.getNbrVertices();
			_nbrBytePool += sizeAttribute;
			_sizeAttribute[index] += sizeAttribute;
			memory.setSizeBlock(index, sizeAttribute);
			if (index > 0)
				_offsetAttribute[index] = _offsetAttribute[index - 1] + _sizeAttribute[index - 1];
		}
		_poolMemory.push_back(memory);
		Element<Vertices> element;
		Key<Element<Vertices>> keyElement;
		element.data = &vertices;
		element.memoryIndex = _poolMemory.size() - 1;
		_poolElement[keyElement] = element;// &vertices, memory);
		return (keyElement);
	}

	Key<Pool::Element<Indices>> IndexPool::addIndices(Indices const &indices)
	{
		// Check in memory pool if a field is free
		for (size_t index = 0; index < _poolMemory.size(); ++index)
		{
			MemoryBlocksGPU &memory = _poolMemory[index];
			if (_poolMemory[index].getIsUsed() == false && memory.getNbrElement() == indices.getNbrIndices())
			{
				_internalSyncronized = false;
				memory.setSync(false);
				memory.setIsUsed(true);
				Key<Element<Indices>> keyElement;
				Element<Indices> newElement;
				newElement.memoryIndex = index;
				newElement.data = &indices;
				_poolElement[keyElement] = newElement;
				return (keyElement);
			}
		}
		// Udpate major is require cause of size pool changement
		_syncronized = false; // cause reset of buffer on gpu
		_internalSyncronized = false;
		for (int index = 0; index < _poolMemory.size(); ++index)
			_poolMemory[index].setSync(false); // ask an update of all element of memory gpu
		MemoryBlocksGPU memory;
		memory.setNbrElement(indices.getNbrIndices());
		memory.setNbrBlock(_nbrAttribute);
		for (uint8_t index = 0; index < _nbrAttribute; ++index)
		{
			size_t sizeAttribute = _sizeTypeComponent[index] * _nbrComponent[index] * indices.getNbrIndices();
			_nbrBytePool += sizeAttribute;
			_sizeAttribute[index] += sizeAttribute;
			memory.setSizeBlock(index, sizeAttribute);
			if (index > 0)
				_offsetAttribute[index] = _offsetAttribute[index - 1] + _sizeAttribute[index - 1];
		}
		_poolMemory.push_back(memory);
		Element<Indices> element;
		Key<Element<Indices>> keyElement;
		element.data = &indices;
		element.memoryIndex = _poolMemory.size() - 1;
		_poolElement[keyElement] = element;// &vertices, memory);
		return (keyElement);
	}

	VertexPool &VertexPool::rmVertices(Key<Pool::Element<Vertices>> const &key)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "rmVertices", "key invalid", *this)
		auto &element = _poolElement.find(key);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU &memory = _poolMemory[element->second.memoryIndex];
		memory.setSync(true);
		memory.setIsUsed(false);
		_poolElement.erase(element);
		return (*this);
	}

	IndexPool &IndexPool::rmIndices(Key<Pool::Element<Indices>> const &key)
	{
		if (!key)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "rmVertices", "key invalid", *this)
			auto &element = _poolElement.find(key);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU &memory = _poolMemory[element->second.memoryIndex];
		memory.setSync(true);
		memory.setIsUsed(false);
		_poolElement.erase(element);
		return (*this);
	}

	void Pool::clearPool()
	{
		_syncronized = true;
		_internalSyncronized = true;
		_nbrBytePool = 0;
		_nbrElementPool = 0;
		_poolMemory.clear();
	}

	VertexPool &VertexPool::clearPool()
	{
		Pool::clearPool();
		memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
		memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		_poolElement.clear();
		return (*this);
	}

	IndexPool &IndexPool::clearPool()
	{
		Pool::clearPool();
		*_sizeAttribute = 0;
		*_offsetAttribute = 0;
		_poolElement.clear();
		return (*this);
	}

	VertexPool::VertexPool()
		: Pool(),
		_indexPoolattach(NULL)
	{
	}

	VertexPool::VertexPool(VertexPool const &copy)
		: Pool(copy),
		_indexPoolattach(copy._indexPoolattach)
	{
	}

	VertexPool::VertexPool(uint8_t nbrAttribute, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
		: Pool(nbrAttribute, typeComponent, sizeTypeComponent, nbrComponent),
		_indexPoolattach(NULL)
	{
	}

	VertexPool::~VertexPool()
	{
	}

	// permit too set the nbr of attribute, set by default all data into it
	// execpt if the nbrAttribute is equal of the value initial
	VertexPool &VertexPool::setNbrAttribute(uint8_t nbrAttribute)
	{
		clearPool();
		if (_nbrAttribute != nbrAttribute)
		{
			if (_nbrAttribute)
			{
				delete[] _typeComponent;
				delete[] _sizeTypeComponent;
				delete[] _nbrComponent;
				delete[] _sizeAttribute;
				delete[] _offsetAttribute;
			}
			_nbrAttribute = nbrAttribute;
			if (_nbrAttribute)
			{
				_typeComponent = new GLenum[_nbrAttribute];
				_sizeTypeComponent = new uint8_t[_nbrAttribute];
				_nbrComponent = new uint8_t[_nbrAttribute];
				_sizeAttribute = new size_t[_nbrAttribute];
				_offsetAttribute = new size_t[_nbrAttribute];
				for (uint8_t index = 0; index < _nbrAttribute; ++index)
				{
					_typeComponent[index] = GL_FLOAT;
					_sizeTypeComponent[index] = sizeof(float);
					_nbrComponent[index] = (index == 2) ? 2 : 4;
				}
			}
			else
			{
				_typeComponent = NULL;
				_sizeTypeComponent = NULL;
				_typeComponent = NULL;
				_sizeTypeComponent = NULL;
				_nbrComponent = NULL;
			}
			memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		}
		return (*this);
	}

	// set type at the attribute index, check the validity of data
	VertexPool &VertexPool::setTypeComponent(uint8_t index, GLenum type)
	{
		if (index >= _nbrAttribute)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "setTypeComponent", "index out of range", *this)
		clearPool();
		_typeComponent[index] = type;
		return (*this);
	}

	// set size type in byte at the attribute index, check the validity of data
	VertexPool &VertexPool::setSizeTypeComponent(uint8_t index, uint8_t sizeType)
	{
		if (index >= _nbrAttribute)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "setSizeTypeComponent", "index out of range", *this)
		clearPool();
		_sizeTypeComponent[index] = sizeType;
		return (*this);
	}

	// set nbr Component at the attribute index, check the validity of data
	VertexPool &VertexPool::setNbrComponent(uint8_t index, uint8_t nbrComponent)
	{
		if (index >= _nbrAttribute)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "setNbrComponent", "index out of range", *this)
		clearPool();
		_nbrComponent[index] = nbrComponent;
		return (*this);
	}

	// warning suppress all data in pool for the new
	VertexPool &VertexPool::setData(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
	{
		clearPool();
		if (_nbrAttribute != nbrAttributes)
		{
			if (_nbrAttribute)
			{
				delete[] _sizeTypeComponent;
				delete[] _typeComponent;
				delete[] _nbrComponent;
				delete[] _sizeAttribute;
				delete[] _offsetAttribute;
			}
			_nbrAttribute = nbrAttributes;
			if (_nbrAttribute)
			{
				_sizeTypeComponent = new uint8_t[_nbrAttribute];
				_typeComponent = new GLenum[_nbrAttribute];
				_nbrComponent = new uint8_t[_nbrAttribute];
				_sizeAttribute = new size_t[_nbrAttribute];
				_offsetAttribute = new size_t[_nbrAttribute];
			}
			else
			{
				_sizeAttribute = NULL;
				_offsetAttribute = NULL;
				_sizeTypeComponent = NULL;
				_typeComponent = NULL;
				_nbrComponent = NULL;
			}
		}
		memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
		memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		memcpy(_sizeTypeComponent, sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
		memcpy(_typeComponent, typeComponent, sizeof(GLenum)* nbrAttributes);
		memcpy(_nbrComponent, nbrComponent, sizeof(uint8_t)* nbrAttributes);
		return (*this);
	}

	Pool &VertexPool::syncronisation()
	{
		if (!_syncronized || !_internalSyncronized)
			_vbo.bind();
		if (!_syncronized)
			_vbo.BufferData(_nbrBytePool);
		if (!_internalSyncronized)
		{
			if (_nbrAttribute)
				memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			_nbrElementPool = 0;
			for (auto &index = _poolElement.begin(); index != _poolElement.end(); ++index)
			{
				Vertices const *vertices = index->second.data;
				MemoryBlocksGPU &memory = _poolMemory[index->second.memoryIndex];
				if (vertices && memory.getSync() == false)
				{
					memory.setStartElement(_nbrElementPool);
					for (uint8_t index = 0; index < _nbrAttribute; ++index)
					{
						memory.setSync(true);
						memory.setOffset(index, _offsetAttribute[index] + _sizeAttribute[index]);
						_vbo.BufferSubData(memory.getOffset(index), memory.getSizeBlock(index), (void *)vertices->getBuffer(index));
						_sizeAttribute[index] += memory.getSizeBlock(index);
					}
					_nbrElementPool += memory.getNbrElement();
				}
			}
		}
		if (!_syncronized)
		{
			_vao.bind();
			_vbo.bind();
			if (_indexPoolattach)
				_indexPoolattach->getBuffer().bind();
			for (size_t index = 0; index < _nbrAttribute; ++index)
			{
				_vao.activateAttribute(index);
				_vao.attribute(index, _nbrComponent[index], _typeComponent[index], _offsetAttribute[index]);
			}
			_vao.unbind();
		}
		_internalSyncronized = true;
		_syncronized = true;
		return (*this);
	}

	VertexPool const &VertexPool::draw(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt) const
	{
		if (!_indexPoolattach && _indexPoolattach->_vertexPoolattach)
			DEBUG_MESSAGE("Warning", "Pool.cpp", "draw", "no indexpool attach", *this)
		_vao.bind();
		auto &element = _poolElement.find(drawOnIt);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->second.memoryIndex];
		_indexPoolattach->onDrawCall(mode, drawWithIt, memory);
		_vao.unbind();
		return (*this);
	}

	VertexPool &VertexPool::operator=(VertexPool const &v)
	{
		if (this != &v)
		{
			clearPool();
			Pool::operator=(v);
			_indexPoolattach = v._indexPoolattach;
			return (*this);
		}
		return (*this);
	}

	VertexPool const &VertexPool::draw(GLenum mode, Key<Element<Vertices>> const &drawIt) const
	{
		_vao.bind();
		auto &element = _poolElement.find(drawIt);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->second.memoryIndex];
		glDrawArrays(mode, GLint(memory.getElementStart()), GLsizei(memory.getNbrElement()));
		_vao.unbind();
		return (*this);
	}

	VertexPool &VertexPool::attachIndexPoolToVertexPool(IndexPool const &pool)
	{
		_indexPoolattach = &pool;
		return (*this);
	}

	VertexPool &VertexPool::dettachIndexPoolToVertexPool()
	{
		_indexPoolattach = NULL;
		return (*this);
	}

	Buffer const & VertexPool::getBuffer() const
	{
		return (_vbo);
	}

	IndexPool::IndexPool()
		: Pool(INDEXPOOL),
		_vertexPoolattach(NULL)
	{
	}

	IndexPool::~IndexPool()
	{
	}

	IndexPool::IndexPool(IndexPool const &copy)
		: Pool(copy),
		_vertexPoolattach(copy._vertexPoolattach)
	{

	}

	IndexPool &IndexPool::operator=(IndexPool const &i)
	{
		if (this != &i)
		{
			clearPool();
			Pool::operator=(i);
			_vertexPoolattach = i._vertexPoolattach;
		}
		return (*this);
	}

	Pool &IndexPool::syncronisation()
	{
		if (!_syncronized || !_internalSyncronized)
			_ibo.bind();
		if (!_syncronized)
			_ibo.BufferData(_nbrBytePool);
		if (!_internalSyncronized)
		{
			if (_nbrAttribute)
				memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			_nbrElementPool = 0;
			for (auto &index = _poolElement.begin(); index != _poolElement.end(); ++index)
			{
				Indices const *indices = index->second.data;
				MemoryBlocksGPU &memory = _poolMemory[index->second.memoryIndex];
				if (indices && memory.getSync() == false)
				{
					memory.setStartElement(_nbrElementPool);
					for (uint8_t index = 0; index < _nbrAttribute; ++index)
					{
						memory.setSync(true);
						memory.setOffset(index, _offsetAttribute[index] + _sizeAttribute[index]);
						_ibo.BufferSubData(memory.getOffset(index), memory.getSizeBlock(index), (void *)indices->getBuffer());
						_sizeAttribute[index] += memory.getSizeBlock(index);
					}
					_nbrElementPool += memory.getNbrElement();
				}
			}
		}
		_internalSyncronized = true;
		_syncronized = true;
		return (*this);
	}

	Buffer const &IndexPool::getBuffer() const
	{
		return (_ibo);
	}

	IndexPool const &IndexPool::onDrawCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target) const
	{
		auto &element = _poolElement.find(key);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->second.memoryIndex];
		glDrawElementsBaseVertex(mode, GLsizei(memory.getNbrElement()), GL_UNSIGNED_INT, (const GLvoid *)(memory.getElementStart() * sizeof(unsigned int)), GLint(target.getElementStart()));
		return (*this);
	}

	IndexPool &IndexPool::attachVertexPoolToIndexPool(VertexPool const &pool)
	{
		_vertexPoolattach = &pool;
		return (*this);
	}

	IndexPool &IndexPool::dettachVertexPoolToIndexPool()
	{
		_vertexPoolattach = NULL;
		return (*this);
	}
}