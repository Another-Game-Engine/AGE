#include <OpenGL/VertexManager.hh>
#include <OpenGL/MemoryBlocksGPU.hh>
#include <OpenGL/Vertice.hh>

#if TEST_NEW_VERTEXMANAGER

#define WARNING_MESSAGE_ATTRIBUTE_SETTING(type) \
	std::cerr << "Warning: setting of [" << type << "] attribute out of attribute range" << std::endl;
#define WARNING_MESSAGE_ATTRIBUTE_GETTING(type) \
	std::cerr << "Warning: getting of [" << type << "] attribute out of attribute range" << std::endl;


namespace gl
{
	VerticesManager::VerticesManager()
	{

	}

	VerticesManager::~VerticesManager()
	{

	}

	Key<VerticesManager::VerticesPool> const &VerticesManager::addPool()
	{
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (!_pools[index].first)
			{
				_pools[index].first = Key<VerticesPool>();
				_pools[index].second = VerticesPool();
				return (_pools[index].first);
			}
		}
		_pools.push_back(std::make_pair(Key<VerticesPool>(), VerticesPool()));
		return (_pools[_pools.size() - 1].first);
	}

	Key<VerticesManager::VerticesPool> const &VerticesManager::addPool(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
	{
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (!_pools[index].first)
			{
				_pools[index].first = Key<VerticesPool>();
				_pools[index].second = VerticesPool();
				return (_pools[index].first);
			}
		}
		_pools.push_back(std::make_pair(Key<VerticesPool>(), VerticesPool()));
		_pools.back().second.setData(nbrAttributes, typeComponent, sizeTypeComponent, nbrComponent);
		return (_pools[_pools.size() - 1].first);
	}

	Key<VerticesManager::VerticesPool> VerticesManager::getPool(size_t index) const
	{
		if (index >= _pools.size())
		{
			Key<VerticesPool> corruptKey;
			corruptKey.destroy();
			return (corruptKey);
		}
		return (_pools[index].first);
	}

	size_t VerticesManager::nbrPool() const
	{
		return (_pools.size());
	}

	// the key will be set to empty
	VerticesManager &VerticesManager::rmPool(Key<VerticesPool> const &key)
	{
		if (!key)
			return (*this);
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (_pools[index].first == key)
			{
				_pools[index].first.destroy();
				return (*this);
			}
		}
		return (*this);
	}

	Key<Vertices> VerticesManager::getVertices(size_t index) const
	{
		if (index >= _vertices.size())
		{
			Key<Vertices> corruptKey;
			corruptKey.destroy();
			return (corruptKey);
		}
		return (_vertices[index].first);
	}

	size_t VerticesManager::getNbrVertices() const
	{
		return (_vertices.size());
	}

	Key<Vertices> const &VerticesManager::addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers)
	{
		for (size_t index = 0; index < _vertices.size(); ++index)
		{
			if (!_vertices[index].first)
			{
				_vertices[index].second = Vertices(nbrVertices, nbrBuffers, sizeBuffers, buffers);
				_vertices[index].first = Key<Vertices>();
				return (_vertices[index].first);
			}
		}
		_vertices.push_back(std::make_pair(Key<Vertices>(), Vertices(nbrVertices, nbrBuffers, sizeBuffers, buffers)));
		return (_vertices[_vertices.size() - 1].first);
	}

	VerticesManager &VerticesManager::rmVertices(Key<Vertices> const &key)
	{
		if (!key)
			return (*this);
		for (size_t index = 0; index < _vertices.size(); ++index)
		{
			if (_vertices[index].first == key)
			{
				_vertices[index].first.destroy();
				return (*this);
			}
		}
		return (*this);
	}

	VerticesManager::VerticesPool::VerticesPool()
		: _nbrAttribute(4),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_nbrBytePool(0),
		_needSync(false)
	{
		if (_nbrAttribute)
		{
			_typeComponent = new GLenum[_nbrAttribute];
			_sizeTypeComponent = new uint8_t[_nbrAttribute];
			_nbrComponent = new uint8_t[_nbrAttribute];
			for (uint8_t index = 0; index < _nbrAttribute; ++index)
			{
				_typeComponent[index] = GL_FLOAT;
				_sizeTypeComponent[index] = sizeof(float);
				_nbrComponent[index] = (index == 2) ? 2 : 4;
			}
		}
	}

	VerticesManager::VerticesPool::VerticesPool(VerticesPool const &copy)
		: _nbrAttribute(copy._nbrAttribute),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_nbrBytePool(copy._nbrBytePool),
		_needSync(copy._needSync)
	{
		if (_nbrAttribute)
		{
			_typeComponent = new GLenum[_nbrAttribute];
			_sizeTypeComponent = new uint8_t[_nbrAttribute];
			_nbrComponent = new uint8_t[_nbrAttribute];
			memcpy(_typeComponent, copy._typeComponent, sizeof(GLenum)* _nbrAttribute);
			memcpy(_sizeTypeComponent, copy._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			memcpy(_nbrComponent, copy._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
		}
	}

	VerticesManager::VerticesPool::~VerticesPool()
	{
		if (_nbrAttribute)
		{
			delete[] _typeComponent;
			delete[] _sizeTypeComponent;
			delete[] _nbrComponent;
		}
	}

	// set all data with "p" data
	VerticesManager::VerticesPool &VerticesManager::VerticesPool::operator=(VerticesPool const &p)
	{
		if (this != &p)
		{
			_nbrBytePool = p._nbrBytePool;
			_needSync = p._needSync;
			if (_nbrAttribute != p._nbrAttribute)
			{
				if (_nbrAttribute)
				{
					delete[] _typeComponent;
					delete[] _sizeTypeComponent;
					delete[] _nbrComponent;
				}
				_nbrAttribute = p._nbrAttribute;
				if (_nbrAttribute)
				{
					_typeComponent = new GLenum[p._nbrAttribute];
					_sizeTypeComponent = new uint8_t[p._nbrAttribute];
					_nbrComponent = new uint8_t[p._nbrAttribute];
				}
				else
				{
					_typeComponent = NULL;
					_sizeTypeComponent = NULL;
					_nbrComponent = NULL;
				}
			}
			memcpy(_typeComponent, p._typeComponent, sizeof(GLenum)* _nbrAttribute);
			memcpy(_sizeTypeComponent, p._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			memcpy(_nbrComponent, p._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
		}
		return (*this);
	}

	// permit too set the nbr of attribute, set by default all data into it
	// execpt if the nbrAttribute is equal of the value initial
	VerticesManager::VerticesPool &VerticesManager::VerticesPool::setNbrAttribute(uint8_t nbrAttribute)
	{
		if (_nbrAttribute != nbrAttribute)
		{
			if (_nbrAttribute)
			{
				delete[] _typeComponent;
				delete[] _sizeTypeComponent;
				delete[] _nbrComponent;
			}
			_nbrAttribute = nbrAttribute;
			if (_nbrAttribute)
			{
				_typeComponent = new GLenum[_nbrAttribute];
				_sizeTypeComponent = new uint8_t[_nbrAttribute];
				_nbrComponent = new uint8_t[_nbrAttribute];
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
				_nbrComponent = NULL;
			}
		}
		return (*this);
	}

	// set type at the attribute index, check the validity of data
	VerticesManager::VerticesPool &VerticesManager::VerticesPool::setTypeComponent(uint8_t index, GLenum type)
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("type")
			return (*this);
		}
		_typeComponent[index] = type;
		return (*this);
	}

	// set size type in byte at the attribute index, check the validity of data
	VerticesManager::VerticesPool &VerticesManager::VerticesPool::setSizeTypeComponent(uint8_t index, uint8_t sizeType)
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("size type")
			return (*this);
		}
		_sizeTypeComponent[index] = sizeType;
		return (*this);
	}

	// set nbr Component at the attribute index, check the validity of data
	VerticesManager::VerticesPool &VerticesManager::VerticesPool::setNbrComponent(uint8_t index, uint8_t nbrComponent)
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("numbre component")
			return (*this);
		}
		_nbrComponent[index] = nbrComponent;
		return (*this);
	}

	// warning the typeComponent and other array send in paramter must have nbrAttribute element
	VerticesManager::VerticesPool &VerticesManager::VerticesPool::setData(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
	{
		if (_nbrAttribute != nbrAttributes)
		{
			if (_nbrAttribute)
			{
				delete[] _sizeTypeComponent;
				delete[] _typeComponent;
				delete[] _nbrComponent;
			}
			_nbrAttribute = nbrAttributes;
			if (_nbrAttribute)
			{
				_sizeTypeComponent = new uint8_t[_nbrAttribute];
				_typeComponent = new GLenum[_nbrAttribute];
				_nbrComponent = new uint8_t[_nbrAttribute];
			}
			else
			{
				_sizeTypeComponent = NULL;
				_typeComponent = NULL;
				_nbrComponent = NULL;
			}
		}
		memcpy(_sizeTypeComponent, sizeTypeComponent, sizeof(uint8_t) * _nbrAttribute);
		memcpy(_typeComponent, typeComponent, sizeof(GLenum) * nbrAttributes);
		memcpy(_nbrComponent, nbrComponent, sizeof(uint8_t) * nbrAttributes);
		return (*this);
	}

	uint8_t VerticesManager::VerticesPool::getNbrAttribute() const
	{
		return (_nbrAttribute);
	}

	GLenum VerticesManager::VerticesPool::getTypeComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_GETTING("type")
			return (-1);
		}
		return (_typeComponent[index]);
	}

	uint8_t VerticesManager::VerticesPool::getSizeTypeComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("size type")
				return (-1);
		}
		return (_sizeTypeComponent[index]);
	}

	uint8_t VerticesManager::VerticesPool::getNbrComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("numbre component")
			return (-1);
		}
		return (_nbrComponent[index]);
	}

	VerticesManager::VerticesPool &VerticesManager::VerticesPool::addVertices(Vertices &vertices)
	{
		// test if a field is empty
		for (size_t index = 0; index < _pool.size(); ++index)
		{
			if (_pool[index].first && vertices.getNbrVertices() == _pool[index].second.getNbrElement())
			{
				_pool[index].first = true;
				vertices.setMemoryBlocksGPU(&(_pool[_pool.size() - 1].second));
				return (*this);
			}
		}
		// no field is free, push a new field to store data
		_needSync = true;
		MemoryBlocksGPU memory;
		memory.setNbrElement(vertices.getNbrVertices());
		memory.setNbrBlock(_nbrAttribute);
		for (size_t index = 0; index < _nbrAttribute; ++index)
		{
			size_t nbrByteAttributes = _sizeTypeComponent[index] * _nbrComponent[index] * vertices.getNbrVertices();
			_nbrBytePool += nbrByteAttributes;
			memory.setSizeBlock(index, nbrByteAttributes);
		}
		if (_nbrAttribute)
		{
			// set start of attribute
			memory.setStartBlock(0, 0);
			for (size_t index = 1; index < _nbrAttribute; ++index)
				memory.setStartBlock(index, memory.getStartBlock(index - 1) + memory.getSizeBlock(index - 1));
			// set the base where start the attribute
			memory.setOffset(0, 0);
			for (size_t index = 1; index < _nbrAttribute; ++index)
				memory.setOffset(index, memory.getOffset(index) + (_sizeTypeComponent[index - 1] * _nbrComponent[index - 1] * vertices.getNbrVertices()));
		}
		_pool.push_back(std::make_pair(true, memory));
		vertices.setMemoryBlocksGPU(&(_pool[_pool.size() - 1].second));
		return (*this);
	}

	VerticesManager::VerticesPool &VerticesManager::VerticesPool::rmVertices(Vertices &vertices)
	{
		MemoryBlocksGPU const *memory = vertices.getMemoryBlocksGPU();
		
		vertices.setMemoryBlocksGPU(NULL);
		for (size_t index = 0; index < _pool.size(); ++index)
		{
			if (memory == &_pool[index].second)
			{
				_pool[index].first = false;
				return (*this);
			}
		}
		return (*this);
	}

}

#endif /*!TEST_NEW_VERTEXMANAGER*/