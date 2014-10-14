#include <Render/Pool.hh>
#include <Render/MemoryGPU.hh>
#include <Render/Data.hh>
#include <iostream>
#include <cassert>

#define INDEXPOOL 1

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
			assert(0);
		return (_typeComponent[index]);
	}

	uint8_t Pool::getSizeTypeComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			assert(0);
		return (_sizeTypeComponent[index]);
	}

	uint8_t Pool::getNbrComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			assert(0);
		return (_nbrComponent[index]);
	}

	size_t Pool::getSizeAttribute(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			assert(0);
			return (_sizeAttribute[index]);
	}

	size_t Pool::getOffsetAttribute(uint8_t index) const
	{
		if (index >= _nbrAttribute)
			assert(0);
		return (_offsetAttribute[index]);
	}


	size_t Pool::getNbrBytePool() const
	{
		return (_nbrBytePool);
	}

	Key<Pool::Element<Vertices>> VertexPool::addVertices(Vertices const &vertices)
	{
		return (addElementPool<Vertices>(_poolElement, vertices));
	}

	Key<Pool::Element<Indices>> IndexPool::addIndices(Indices const &indices)
	{
		return (addElementPool<Indices>(_poolElement, indices));
	}

	Pool::Element<Vertices> const *VertexPool::getVerticesPoolElement(Key<Element<Vertices>> const &key) const
	{
		if (!key)
			assert(0);
		auto &element = _poolElement.find(key);
		if (element == _poolElement.end())
			return (NULL);
		return (&element->second);
	}

	Pool::Element<Indices> const *IndexPool::getIndicesPoolElement(Key<Element<Indices>> const &key) const
	{
		if (!key)
			assert(0);
		auto &element = _poolElement.find(key);
		if (element == _poolElement.end())
			return (NULL);
		return (&element->second);
	}

	VertexPool &VertexPool::rmVertices(Key<Pool::Element<Vertices>> &key)
	{
		Pool::Element<Vertices> const *element;
		if ((element = getVerticesPoolElement(key/*, "rmVertices"*/)) == NULL)
			return (*this);
		MemoryBlocksGPU &memory = _poolMemory[element->memoryIndex];
		memory.setSync(true);
		memory.setIsUsed(false);
		_poolElement.erase(key);
		key.destroy();
		return (*this);
	}

	IndexPool &IndexPool::rmIndices(Key<Pool::Element<Indices>> &key)
	{
		Pool::Element<Indices> const *element;
		if ((element = getIndicesPoolElement(key/*, "rmIndices"*/)) == NULL)
			return (*this);
		MemoryBlocksGPU &memory = _poolMemory[element->memoryIndex];
		memory.setSync(true);
		memory.setIsUsed(false);
		_poolElement.erase(key);
		key.destroy();
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
			clear();
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
			assert(0);
		clearPool();
		_typeComponent[index] = type;
		return (*this);
	}

	// set size type in byte at the attribute index, check the validity of data
	VertexPool &VertexPool::setSizeTypeComponent(uint8_t index, uint8_t sizeType)
	{
		if (index >= _nbrAttribute)
			assert(0);
		clearPool();
		_sizeTypeComponent[index] = sizeType;
		return (*this);
	}

	// set nbr Component at the attribute index, check the validity of data
	VertexPool &VertexPool::setNbrComponent(uint8_t index, uint8_t nbrComponent)
	{
		if (index >= _nbrAttribute)
			assert(0);
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
			clear();
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
		if (!_indexPoolattach) assert(0);
		_vao.bind();
		auto &element = _poolElement.find(drawOnIt);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->second.memoryIndex];
		_indexPoolattach->onDrawCall(mode, drawWithIt, memory);
		_vao.unbind();
		return (*this);
	}

	VertexPool const &VertexPool::drawInstanced(GLenum mode, Key<Element<Indices>> const &drawWithIt, Key<Element<Vertices>> const &drawOnIt, size_t nbrIntanced) const
	{
		if (!_indexPoolattach) assert(0);
		_vao.bind();
		auto &element = _poolElement.find(drawOnIt);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->second.memoryIndex];
		_indexPoolattach->onDrawIntancedCall(mode, drawWithIt, memory, nbrIntanced);
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
		Pool::Element<Vertices> const *element;
		if ((element = getVerticesPoolElement(drawIt/*, "draw"*/)) == NULL)
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->memoryIndex];
		_vao.bind();
		glDrawArrays(mode, GLint(memory.getElementStart()), GLsizei(memory.getNbrElement()));
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
		Pool::Element<Indices> const *element;
		if ((element = getIndicesPoolElement(key/*, "rmIndices"*/)) == NULL)
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->memoryIndex];
		glDrawElementsBaseVertex(mode, GLsizei(memory.getNbrElement()), GL_UNSIGNED_INT, (const GLvoid *)(memory.getElementStart() * sizeof(unsigned int)), GLint(target.getElementStart()));
		return (*this);
	}

	IndexPool const &IndexPool::onDrawIntancedCall(GLenum mode, Key<Element<Indices>> const &key, MemoryBlocksGPU const &target, size_t nbrIntanced) const
	{
		Pool::Element<Indices> const *element;
		if ((element = getIndicesPoolElement(key/*, "rmIndices"*/)) == NULL)
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory[element->memoryIndex];
		glDrawElementsInstancedBaseVertex(mode, GLsizei(memory.getNbrElement()), GL_UNSIGNED_INT, (const GLvoid *)(memory.getElementStart() * sizeof(unsigned int)), nbrIntanced, GLint(target.getElementStart()));
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