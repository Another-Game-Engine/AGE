#include <OpenGL/VertexManager.hh>

#if TEST_NEW_VERTEXMANAGER

namespace gl
{
	VerticesManager::VerticesManager()
	{

	}

	VerticesManager::~VerticesManager()
	{

	}

	Key<VerticesPool> const &VerticesManager::addPool(VerticesPool const &pool)
	{
		for (size_t index = 0; index < _pools.size(); ++index)
		{
			if (_pools[index].first == false)
			{
				_pools[index].first = true;
				_pools[index].second = pool;
				return (Key<VerticesPool>(index));
			}
		}
		_pools.push_back(std::make_pair(true, pool));
		return (Key<VerticesPool>(_pools.size() - 1));
	}

	Key<VerticesPool> const &VerticesManager::getPool(size_t index) const
	{
		if (index >= _pools.size())
			return (Key<VerticesPool>(-1));
		return (Key<VerticesPool>(index));
	}

	size_t VerticesManager::nbrPool() const
	{
		return (_pools.size());
	}

	void VerticesManager::rmPool(Key<VerticesPool> const &key)
	{
		if ((key.getId() < _pools.size()) && key.empty() == false)
			_pools[key.getId()].first = false;
	}

	void VerticesManager::clearPool()
	{
		_pools.clear();
	}

	Key<Vertices> const &VerticesManager::addVertices(Vertices const &vertices)
	{
		for (size_t index = 0; index < _vertices.size(); ++index)
		{
			if (_vertices[index].first == false)
			{
				_vertices[index].first = true;
				_vertices[index].second = vertices;
				return (Key<Vertices>(index));
			}
		}
		_vertices.push_back(std::make_pair(true, vertices));
		return (Key<Vertices>(_vertices.size() - 1));
	}

	void VerticesManager::rmVertices(Key<Vertices> const &key)
	{
		if ((key.getId() < _vertices.size()) && key.empty() == false)
			_vertices[key.getId()].first = false;
	}

	Key<Vertices> const &VerticesManager::getVertices(size_t index) const
	{
		if (index >= _vertices.size())
			return (Key<Vertices>(-1));
		return (Key<Vertices>(index));
	}

	size_t VerticesManager::nbrVertices() const
	{
		return (_vertices.size());
	}

	void VerticesManager::clearVertices()
	{
		_vertices.clear();
	}

	VerticesPool::VerticesPool()
		: _nbrAttribute(4),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL)
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

	VerticesPool::VerticesPool(VerticesPool const &copy)
		: _nbrAttribute(copy._nbrAttribute),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_poolElement(copy._poolElement)
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

	VerticesPool::~VerticesPool()
	{
		if (_nbrAttribute)
		{
			delete[] _typeComponent;
			delete[] _sizeTypeComponent;
			delete[] _nbrComponent;
		}
	}

	// set all data with "p" data
	VerticesPool &VerticesPool::operator=(VerticesPool const &p)
	{
		if (this != &p)
		{
			if (_nbrAttribute != p._nbrAttribute && p._nbrAttribute)
			{
				if (_nbrAttribute)
				{
					delete[] _typeComponent;
					delete[] _sizeTypeComponent;
					delete[] _nbrComponent;
				}
				_typeComponent = new GLenum[p._nbrAttribute];
				_sizeTypeComponent = new uint8_t[p._nbrAttribute];
				_nbrComponent = new uint8_t[p._nbrAttribute];
			}
			_nbrAttribute = p._nbrAttribute;
			memcpy(_typeComponent, p._typeComponent, sizeof(GLenum)* _nbrAttribute);
			memcpy(_sizeTypeComponent, p._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			memcpy(_nbrComponent, p._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
			_poolElement = p._poolElement;
		}
		return (*this);
	}

	// permit too set the nbr of attribute, set by default all data into it
	// execpt if the nbrAttribute is equal of the value initial
	VerticesPool &VerticesPool::setNbrAttribute(uint8_t nbrAttribute)
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
			if (nbrAttribute)
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
		return (*this);
	}

	// set type at the attribute index, check the validity of data
	VerticesPool &VerticesPool::setTypeComponent(uint8_t index, GLenum type)
	{
		if (index <= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("type")
			return (*this);
		}
		_typeComponent[index] = type;
		return (*this);
	}

	// set size type in byte at the attribute index, check the validity of data
	VerticesPool &VerticesPool::setSizeTypeComponent(uint8_t index, uint8_t sizeType)
	{
		if (index <= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("size type")
			return (*this);
		}
		_sizeTypeComponent[index] = sizeType;
		return (*this);
	}

	// set nbr Component at the attribute index, check the validity of data
	VerticesPool &VerticesPool::setNbrComponent(uint8_t index, uint8_t nbrComponent)
	{
		if (index <= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("numbre component")
			return (*this);
		}
		_nbrComponent[index] = nbrComponent;
		return (*this);
	}

	uint8_t VerticesPool::getNbrAttribute() const
	{
		return (_nbrAttribute);
	}

	GLenum VerticesPool::getTypeComponent(uint8_t index) const
	{
		if (index <= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_GETTING("type")
			return (-1);
		}
		return (_typeComponent[index]);
	}

	uint8_t VerticesPool::getSizeTypeComponent(uint8_t index) const
	{
		if (index <= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("size type")
				return (-1);
		}
		return (_sizeTypeComponent[index]);
	}

	uint8_t VerticesPool::getNbrComponent(uint8_t index) const
	{
		if (index <= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("numbre component")
			return (-1);
		}
		return (_nbrComponent[index]);
	}

}

#endif /*!TEST_NEW_VERTEXMANAGER*/