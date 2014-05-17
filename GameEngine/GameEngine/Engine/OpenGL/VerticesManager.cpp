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
				_pools[index].second = VerticesPool(*this, TypePool::Vertices);
				return (_pools[index].first);
			}
		}
		_pools.push_back(std::make_pair(Key<VerticesPool>(), VerticesPool(*this, TypePool::Vertices)));
		return (_pools[_pools.size() - 1].first);
	}

	Key<VerticesManager::VerticesPool> const &VerticesManager::addPool(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
	{
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (!_pools[index].first)
			{
				_pools[index].first = Key<VerticesPool>();
				_pools[index].second = VerticesPool(*this, TypePool::Vertices);
				return (_pools[index].first);
			}
		}
		_pools.push_back(std::make_pair(Key<VerticesPool>(), VerticesPool(*this, TypePool::Vertices)));
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

	VerticesManager::VerticesPool const * VerticesManager::getPool(Key<VerticesPool> const &key) const
	{
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (_pools[index].first == key)
				return (&_pools[index].second);
		}
		return (NULL);
	}

	// the key will be set to empty
	void VerticesManager::rmPool(Key<VerticesPool> const &key)
	{
		if (!key)
			return;
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (_pools[index].first == key)
			{
				_pools[index].first.destroy();
				return;
			}
		}
	}

	VerticesManager::VerticesPool::VerticesPool(VerticesManager const &database, TypePool type)
		: _nbrAttribute(4),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_database(database),
		_type(type)
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
		_database(copy._database),
		_type(copy._type)
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
		_type = p._type;
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

	VerticesManager::VerticesPool &VerticesManager::VerticesPool::addVertices(Vertices const &vertices)
	{

		MemoryBlocksGPU memory;

		memory.setNbrBlock(_nbrAttribute);
		for (size_t index = 0; index < _nbrAttribute; ++index)
			memory.setSizeBlock(index, _sizeTypeComponent[index] * _nbrComponent[index] * vertices);
		return (*this);
	}

	VerticesManager::VerticesPool &VerticesManager::VerticesPool::rmVertices(Key<Vertices> const &vertices)
	{
		return (*this);
	}

	Key<Vertices> const &VerticesManager::VerticesPool::getVertice(size_t index) const
	{
		if (index >= _pool.size())
		{
			Key<Vertices> corrupt;
			corrupt.destroy();
			return (corrupt);
		}
		return (_pool[index].first);
	}
}

#endif /*!TEST_NEW_VERTEXMANAGER*/