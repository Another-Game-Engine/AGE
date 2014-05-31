#include <OpenGL/Pool.hh>
#include <OpenGL/MemoryBlocksGPU.hh>
#include <OpenGL/Vertices.hh>
#include <iostream>
#include <string>

#define DEBUG_MESSAGE(type, from, key_word, reason) \
	std::cerr << std::string(type) + ":from[" + std::string(from) + "].key-word[" + std::string(key_word) + "].reason[" + std::string(reason) + "]" << std::endl;

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
		_syncronized(true),
		_internalSyncronized(true)
	{
		_typeComponent = new GLenum[_nbrAttribute];
		_sizeTypeComponent = new uint8_t[_nbrAttribute];
		_nbrComponent = new uint8_t[_nbrAttribute];
		_sizeAttribute = new size_t[_nbrAttribute];
		memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
		_offsetAttribute = new size_t[_nbrAttribute];
		memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		for (uint8_t index = 0; index < _nbrAttribute; ++index)
		{
			_typeComponent[index] = GL_FLOAT;
			_sizeTypeComponent[index] = sizeof(float);
			_nbrComponent[index] = (index == 2) ? 2 : 4;
		}
	}

	Pool::Pool(IndexPool const &pool)
		: _nbrAttribute(1),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_sizeAttribute(NULL),
		_offsetAttribute(NULL),
		_nbrBytePool(0),
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
		_syncronized(true),
		_internalSyncronized(true)
	{
		if (_nbrAttribute)
		{
			_typeComponent = new GLenum[_nbrAttribute];
			_sizeTypeComponent = new uint8_t[_nbrAttribute];
			_nbrComponent = new uint8_t[_nbrAttribute];
			_sizeAttribute = new size_t[_nbrAttribute];
			_offsetAttribute = new size_t[_nbrAttribute];
			memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
			memcpy(_typeComponent, typeComponent, sizeof(GLenum)* _nbrAttribute);
			memcpy(_sizeTypeComponent, sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			memcpy(_nbrComponent, nbrComponent, sizeof(uint8_t)* _nbrAttribute);
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
		_syncronized(true)
	{
		if (_nbrAttribute)
		{
			_typeComponent = new GLenum[_nbrAttribute];
			_sizeTypeComponent = new uint8_t[_nbrAttribute];
			_nbrComponent = new uint8_t[_nbrAttribute];
			_sizeAttribute = new size_t[_nbrAttribute];
			_offsetAttribute = new size_t[_nbrAttribute];
			memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
			memcpy(_typeComponent, copy._typeComponent, sizeof(GLenum)* _nbrAttribute);
			memcpy(_sizeTypeComponent, copy._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			memcpy(_nbrComponent, copy._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
		}
	}

	Pool::~Pool()
	{
		clearPool();
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
			clearPool();
			if (_nbrAttribute != p._nbrAttribute)
			{
				if (_nbrAttribute)
				{
					delete[] _typeComponent;
					delete[] _sizeTypeComponent;
					delete[] _nbrComponent;
					delete[] _sizeAttribute;
					delete[] _offsetAttribute;
				}
				_nbrAttribute = p._nbrAttribute;
				if (_nbrAttribute)
				{
					_typeComponent = new GLenum[p._nbrAttribute];
					_sizeTypeComponent = new uint8_t[p._nbrAttribute];
					_nbrComponent = new uint8_t[p._nbrAttribute];
					_sizeAttribute = new size_t[_nbrAttribute];
					_offsetAttribute = new size_t[_nbrAttribute];
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
			memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
			memcpy(_typeComponent, p._typeComponent, sizeof(GLenum)* _nbrAttribute);
			memcpy(_sizeTypeComponent, p._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			memcpy(_nbrComponent, p._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
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
		{
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getTypeComponent", "Index out of range")
			return (-1);
		}
		return (_typeComponent[index]);
	}

	uint8_t Pool::getSizeTypeComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getSizeTypeComponent", "Index out of range")
			return (-1);
		}
		return (_sizeTypeComponent[index]);
	}

	uint8_t Pool::getNbrComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getNbrComponent", "Index out of range")
			return (-1);
		}
		return (_nbrComponent[index]);
	}

	size_t Pool::getSizeAttribute(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getSizeAttribute", "Index out of range")
			return (-1);
		}
		return (_sizeAttribute[index]);
	}

	size_t Pool::getOffsetAttribute(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			DEBUG_MESSAGE("Warning", "Pool.cpp", "getOffsetAttribute", "Index out of range")
			return (-1);
		}
		return (_offsetAttribute[index]);
	}

	size_t Pool::getNbrBytePool() const
	{
		return (_nbrBytePool);
	}


	Key<Pool::PoolElement> const &Pool::addVertices(Vertices const &vertices)
	{
		// Check if the vertices is not already in the pool
		for (auto &index = _poolElement.begin(); index != _poolElement.end(); ++index)
		{
			if (index->second.vertices == &vertices)
			{
				MemoryBlocksGPU &memoryfind = _poolMemory[index->second.memoryKey];
				memoryfind.setNbrElement(memoryfind.getNbrObject() + 1);
				return (index->first);
			}
		}
		// Next check in memory pool if a field is free
		for (auto &index = _poolMemory.begin(); index != _poolMemory.end(); ++index)
		{
			MemoryBlocksGPU &memory = index->second;
			if (memory.getNbrObject() == 0 && memory.getNbrElement() == vertices.getNbrVertices())
			{
				_internalSyncronized = false;
				memory.setSync(false);
				Key<PoolElement> keyElement;
				PoolElement newElement;
				newElement.memoryKey = index->first;
				newElement.vertices = &vertices;
				_poolElement[keyElement] = newElement;
				return (keyElement);
			}
		}
		// Udpate major is require cause of size pool changement
		_syncronized = false; // cause reset of buffer on gpu
		for (auto &index = _poolMemory.begin(); index != _poolMemory.end(); ++index)
			index->second.setSync(false); // ask an update of all element of memory gpu
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
				_offsetAttribute[index] = _sizeAttribute[index - 1];
		}
		PoolElement element;
		Key<PoolElement> keyElement;
		element.vertices = &vertices;
		_poolElement[keyElement] = element;// &vertices, memory);
		return (keyElement);
	}

	Pool &Pool::rmVertices(Key<PoolElement> const &key)
	{
		auto &element = _poolElement.find(key);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU &memory = _poolMemory[element->second.memoryKey];		
		memory.setNbrObject(memory.getNbrObject() - 1);
		if (memory.getNbrObject() == 0)
			memory.setSync(true);
		_poolElement.erase(element);
		return (*this);
	}

	Pool &Pool::clearPool()
	{
		_syncronized = true;
		_internalSyncronized = true;
		_nbrBytePool = 0;
		memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
		memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		_poolElement.clear();
		_poolMemory.clear();
		return (*this);
	}

	void Pool::syncronizeVertices(Vertices const &vertices, MemoryBlocksGPU &memory)
	{
		if (_nbrAttribute)
			memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
		for (uint8_t index = 0; index < _nbrAttribute; ++index)
		{
			memory.setSync(true);
			size_t base = _offsetAttribute[index] + _sizeAttribute[index];
			memory.setOffset(index, base + _sizeAttribute[index]);
			glBufferSubData(_vbo.getMode(), memory.getOffset(index), memory.getSizeBlock(index), vertices.getBuffer(index));
			_sizeAttribute[index] += memory.getSizeBlock(index);
		}
	}
	
	VertexBuffer const &Pool::getVertexBuffer() const
	{
		return (_vbo);
	}

	VertexPool::VertexPool()
		: Pool()
	{
	}

	VertexPool::VertexPool(VertexPool const &copy)
		: Pool(copy)
	{
	}

	VertexPool::VertexPool(uint8_t nbrAttribute, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
		: Pool(nbrAttribute, typeComponent, sizeTypeComponent, nbrComponent)
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
		{
			DEBUG_MESSAGE("Warning", "Pool.cpp", "setTypeComponent", "index out of range")
			return (*this);
		}
		clearPool();
		_typeComponent[index] = type;
		return (*this);
	}

	// set size type in byte at the attribute index, check the validity of data
	VertexPool &VertexPool::setSizeTypeComponent(uint8_t index, uint8_t sizeType)
	{
		if (index >= _nbrAttribute)
		{
			DEBUG_MESSAGE("Warning", "Pool.cpp", "setSizeTypeComponent", "index out of range")
			return (*this);
		}
		clearPool();
		_sizeTypeComponent[index] = sizeType;
		return (*this);
	}

	// set nbr Component at the attribute index, check the validity of data
	VertexPool &VertexPool::setNbrComponent(uint8_t index, uint8_t nbrComponent)
	{
		if (index >= _nbrAttribute)
		{
			DEBUG_MESSAGE("Warning", "Pool.cpp", "setNbrComponent", "index out of range")
			return (*this);
		}
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
		_vbo.bind();
		if (!_syncronized)
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nbrBytePool, NULL, GL_STREAM_DRAW);
		if (!_internalSyncronized)
		{
			for (auto &index = _poolElement.begin(); index != _poolElement.end(); ++index)
			{
				if (index->second.vertices && _poolMemory[index->second.memoryKey].getSync() == false)
					syncronizeVertices(*(index->second.vertices), _poolMemory[index->second.memoryKey]);
			}
		}
		if (!_syncronized)
		{
			_vao.bind();
			_vbo.bind();
			if (_indexPoolattach)
				_indexPoolattach->getVertexBuffer().bind();
			for (size_t index = 0; index < _nbrAttribute; ++index)
			{
				glEnableVertexAttribArray(GLuint(index));
				glVertexAttribPointer(GLuint(index), GLint(_nbrComponent[index]), _typeComponent[index], GL_FALSE, 0, (const GLvoid *)_offsetAttribute[index]);
			}
			_vao.unbind();
		}
		_internalSyncronized = true;
		_syncronized = true;
		return (*this);
	}

	VertexPool const &VertexPool::draw(Key<PoolElement> const &drawWithIt, Key<PoolElement> const &drawOnIt) const
	{
		if (_indexPoolattach)
			return (*this);
		_vao.bind();
		auto &element = _poolElement.find(drawOnIt);
		if (element != _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory.find(element->second.memoryKey)->second;
		_indexPoolattach->draw(drawWithIt, memory);
		_vao.unbind();
		return (*this);
	}

	VertexPool const &VertexPool::draw(Key<PoolElement> const &drawIt) const
	{
		_vao.bind();
		auto &element = _poolElement.find(drawIt);
		if (element == _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory.find(element->second.memoryKey)->second;
		glDrawArrays(GL_TRIANGLES, memory.getElementStart(), memory.getNbrElement());
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

	IndexPool::IndexPool()
		: Pool(IndexPool())
	{
	}

	IndexPool::~IndexPool()
	{
	}

	IndexPool::IndexPool(IndexPool const &copy)
		: Pool(copy)
	{

	}

	IndexPool &IndexPool::operator=(IndexPool const &i)
	{
		return (*this);
	}

	Pool &IndexPool::syncronisation()
	{
		_vbo.bind();
		if (!_syncronized)
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, _nbrBytePool, NULL, GL_STREAM_DRAW);
		if (!_internalSyncronized)
		{
			for (auto &index = _poolElement.begin(); index != _poolElement.end(); ++index)
			{
				if (index->second.vertices && _poolMemory[index->second.memoryKey].getSync() == false)
					syncronizeVertices(*(index->second.vertices), _poolMemory[index->second.memoryKey]);
			}
		}
		_internalSyncronized = true;
		_syncronized = true;
		return (*this);
	}

	IndexPool const &IndexPool::draw(Key<PoolElement> const &key, MemoryBlocksGPU const &target) const
	{
		auto &element = _poolElement.find(key);
		if (element != _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory.find(element->second.memoryKey)->second;
		glDrawElementsBaseVertex(GL_TRIANGLES, memory.getNbrElement(), GL_UNSIGNED_INT, (GLvoid const *)memory.getElementStart(), target.getElementStart());
		return (*this);
	}

	IndexPool const &IndexPool::draw(Key<PoolElement> const &key) const
	{
		_vbo.bind();
		auto &element = _poolElement.find(key);
		if (element != _poolElement.end())
			return (*this);
		MemoryBlocksGPU const &memory = _poolMemory.find(element->second.memoryKey)->second;
		glDrawElements(GL_TRIANGLES, memory.getNbrElement(), GL_UNSIGNED_INT, (GLvoid const *)memory.getElementStart());
	}
}