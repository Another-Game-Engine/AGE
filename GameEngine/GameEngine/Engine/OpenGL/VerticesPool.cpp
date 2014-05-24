#include <OpenGL/VerticesPool.hh>
#include <OpenGL/MemoryBlocksGPU.hh>
#include <OpenGL/Vertices.hh>
#include <stdint.h>
#include <iostream>

#define WARNING_MESSAGE_ATTRIBUTE_SETTING(type) \
	std::cerr << "Warning: setting of [" << type << "] attribute out of attribute range" << std::endl;
#define WARNING_MESSAGE_ATTRIBUTE_GETTING(type) \
	std::cerr << "Warning: getting of [" << type << "] attribute out of attribute range" << std::endl;

namespace gl
{

	VerticesPool::VerticesPool()
		: _nbrAttribute(4),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_sizeAttribute(NULL),
		_offsetAttribute(NULL),
		_nbrBytePool(0),
		_needSync(false)
	{
		if (_nbrAttribute)
		{
			_typeComponent = new GLenum[_nbrAttribute];
			_sizeTypeComponent = new uint8_t[_nbrAttribute];
			_nbrComponent = new uint8_t[_nbrAttribute];
			_sizeAttribute = new size_t[_nbrAttribute];
			memset(_sizeAttribute, 0, sizeof(size_t)* _nbrAttribute);
			_offsetAttribute = new size_t[_nbrAttribute];
			memset(_offsetAttribute, 0, sizeof(size_t) * _nbrAttribute);
			for (uint8_t index = 0; index < _nbrAttribute; ++index)
			{
				_typeComponent[index] = GL_FLOAT;
				_sizeTypeComponent[index] = sizeof(float);
				_nbrComponent[index] = (index == 2) ? 2 : 4;
			}
		}
	}

	VerticesPool::VerticesPool(uint8_t nbrAttribute, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
		: _nbrAttribute(nbrAttribute),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_sizeAttribute(NULL),
		_offsetAttribute(NULL),
		_nbrBytePool(0),
		_needSync(0)
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
			std::memcpy(_typeComponent, typeComponent, sizeof(GLenum)* _nbrAttribute);
			std::memcpy(_sizeTypeComponent, sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			std::memcpy(_nbrComponent, nbrComponent, sizeof(uint8_t)* _nbrAttribute);
		}
	}

	VerticesPool::VerticesPool(VerticesPool const &copy)
		: _nbrAttribute(copy._nbrAttribute),
		_typeComponent(NULL),
		_sizeTypeComponent(NULL),
		_nbrComponent(NULL),
		_sizeAttribute(NULL),
		_offsetAttribute(NULL),
		_nbrBytePool(copy._nbrBytePool),
		_needSync(copy._needSync)
	{
		if (_nbrAttribute)
		{
			_typeComponent = new GLenum[_nbrAttribute];
			_sizeTypeComponent = new uint8_t[_nbrAttribute];
			_nbrComponent = new uint8_t[_nbrAttribute];
			_sizeAttribute = new size_t[_nbrAttribute];
			_offsetAttribute = new size_t[_nbrAttribute];
			std::memcpy(_sizeAttribute, copy._sizeAttribute, sizeof(size_t)* _nbrAttribute);
			std::memcpy(_offsetAttribute, copy._offsetAttribute, sizeof(size_t)* _nbrAttribute);
			std::memcpy(_typeComponent, copy._typeComponent, sizeof(GLenum)* _nbrAttribute);
			std::memcpy(_sizeTypeComponent, copy._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			std::memcpy(_nbrComponent, copy._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
		}
	}

	VerticesPool::~VerticesPool()
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
	VerticesPool &VerticesPool::operator=(VerticesPool const &p)
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
			std::memcpy(_sizeAttribute, p._sizeAttribute, sizeof(size_t)* _nbrAttribute);
			std::memcpy(_offsetAttribute, p._offsetAttribute, sizeof(size_t)* _nbrAttribute);
			std::memcpy(_typeComponent, p._typeComponent, sizeof(GLenum)* _nbrAttribute);
			std::memcpy(_sizeTypeComponent, p._sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
			std::memcpy(_nbrComponent, p._nbrComponent, sizeof(uint8_t)* _nbrAttribute);
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
	VerticesPool &VerticesPool::setTypeComponent(uint8_t index, GLenum type)
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
	VerticesPool &VerticesPool::setSizeTypeComponent(uint8_t index, uint8_t sizeType)
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
	VerticesPool &VerticesPool::setNbrComponent(uint8_t index, uint8_t nbrComponent)
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
	VerticesPool &VerticesPool::setData(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent)
	{
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
		std::memcpy(_sizeTypeComponent, sizeTypeComponent, sizeof(uint8_t)* _nbrAttribute);
		std::memcpy(_typeComponent, typeComponent, sizeof(GLenum)* nbrAttributes);
		std::memcpy(_nbrComponent, nbrComponent, sizeof(uint8_t)* nbrAttributes);
		return (*this);
	}

	uint8_t VerticesPool::getNbrAttribute() const
	{
		return (_nbrAttribute);
	}

	GLenum VerticesPool::getTypeComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_GETTING("type")
				return (-1);
		}
		return (_typeComponent[index]);
	}

	uint8_t VerticesPool::getSizeTypeComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("size type")
				return (-1);
		}
		return (_sizeTypeComponent[index]);
	}

	uint8_t VerticesPool::getNbrComponent(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_SETTING("numbre component")
				return (-1);
		}
		return (_nbrComponent[index]);
	}

	size_t VerticesPool::getSizeAttribute(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_GETTING("size attribute")
				return (-1);
		}
		return (_sizeAttribute[index]);
	}

	size_t VerticesPool::getOffsetAttribute(uint8_t index) const
	{
		if (index >= _nbrAttribute)
		{
			WARNING_MESSAGE_ATTRIBUTE_GETTING("offset attribute")
			return (-1);
		}
		return (_offsetAttribute[index]);
	}

	size_t VerticesPool::getNbrBytePool() const
	{
		return (_nbrBytePool);
	}


	VerticesPool &VerticesPool::addVertices(Vertices &vertices)
	{
		// test if a field is empty
		for (size_t index = 0; index < _pool.size(); ++index)
		{
			if (_pool[index].first && vertices.getNbrVertices() == _pool[index].second.getNbrElement())
			{
				vertices._indexOnPool = index;
				_pool[index].first = &vertices;
				return (*this);
			}
		}
		// no one field is free, push a new field to store data
		_needSync = true;
		MemoryBlocksGPU memory;
		memory.setNbrElement(vertices.getNbrVertices());
		memory.setNbrBlock(_nbrAttribute);
		for (size_t index = 0; index < _nbrAttribute; ++index)
		{
			size_t sizeAttribute = _sizeTypeComponent[index] * _nbrComponent[index] * vertices.getNbrVertices();
			_nbrBytePool += sizeAttribute;
			_sizeAttribute[index] += sizeAttribute;
			memory.setSizeBlock(index, sizeAttribute);
			if (index > 0)
				_offsetAttribute[index] = _sizeAttribute[index - 1];
		}
		_pool.push_back(std::make_pair(&vertices, MemoryBlocksGPU(memory)));// &vertices, memory);
		vertices._indexOnPool = _pool.size() - 1;
		return (*this);
	}

	VerticesPool &VerticesPool::rmVertices(Vertices &vertices)
	{
		_pool[vertices._indexOnPool].first->_indexOnPool = 0;
		_pool[vertices._indexOnPool].first->_pool = NULL;
		_pool[vertices._indexOnPool].first = NULL;
		return (*this);
	}

	VerticesPool &VerticesPool::clearPool()
	{
		_needSync = false;
		_nbrBytePool = 0;
		memset(_sizeAttribute, 0, sizeof(size_t) * _nbrAttribute);
		memset(_offsetAttribute, 0, sizeof(size_t)* _nbrAttribute);
		for (size_t index = 0; index < _pool.size(); ++index)
		{
			if (_pool[index].first)
			{
				_pool[index].first->_indexOnPool = NULL;
				_pool[index].first->_pool = NULL;
			}
		}
		_pool.clear();
		return (*this);
	}

}