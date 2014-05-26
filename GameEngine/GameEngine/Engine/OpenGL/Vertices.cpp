#include "Vertices.hh"
#include <iostream>

namespace gl
{
	Vertices::Vertices()
		: _buffers(NULL),
		_sizeBuffers(NULL),
		_nbrVertices(0),
		_nbrBuffers(0),
		_indexOnPool(0),
		_pool(NULL)
	{
	}

	// nbrVertices is the number will be draw during a render, nbrBuffers is each buffer use for each attribute,
	// sizeBuffer indicate for each buffer is size in byte, buffers contain all data use for the draw for the attribute.
	Vertices::Vertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers)
		: _buffers(NULL),
		_sizeBuffers(NULL),
		_nbrVertices(nbrVertices),
		_nbrBuffers(nbrBuffers),
		_indexOnPool(0),
		_pool(NULL)
	{
		if (_nbrBuffers)
		{
			_buffers = new void*[_nbrBuffers];
			_sizeBuffers = new size_t[_nbrBuffers];
			memcpy(_sizeBuffers, sizeBuffers, sizeof(size_t)* _nbrBuffers);
			for (size_t index = 0; index < _nbrBuffers; ++index)
			{
				_buffers[index] = new uint8_t[sizeBuffers[index]];
				memcpy(_buffers[index], buffers[index], _sizeBuffers[index]);
			}
		}
	}

	Vertices::Vertices(Vertices const &copy)
		: _buffers(NULL),
		_sizeBuffers(NULL),
		_nbrVertices(copy._nbrVertices),
		_nbrBuffers(copy._nbrBuffers),
		_indexOnPool(copy._indexOnPool),
		_pool(copy._pool)
	{
		if (_nbrBuffers)
		{
			_buffers = new void*[_nbrBuffers];
			_sizeBuffers = new size_t[_nbrBuffers];
			memcpy(_sizeBuffers, copy._sizeBuffers, sizeof(size_t)* _nbrBuffers);
			for (size_t index = 0; index < _nbrBuffers; ++index)
			{
				_buffers[index] = new uint8_t[_sizeBuffers[index]];
				memcpy(_buffers[index], copy._buffers[index], _sizeBuffers[index]);
			}
		}
	}

	Vertices::~Vertices()
	{
		if (_nbrBuffers)
		{
			for (size_t index = 0; index < _nbrBuffers; ++index)
				delete[] _buffers[index];
			delete[] _sizeBuffers;
			delete[] _buffers;
		}
		if (_pool != NULL)
			_pool->rmVertices(*this);
	}

	Vertices &Vertices::operator=(Vertices const &v)
	{
		_nbrVertices = v._nbrVertices;
		if (_nbrBuffers != v._nbrBuffers)
		{
			if (_nbrBuffers)
			{
				for (size_t index = 0; index < _nbrBuffers; ++index)
					delete[] _buffers[index];
				delete[] _sizeBuffers;
				delete[] _buffers;
			}
			_nbrBuffers = v._nbrBuffers;
			if (_nbrBuffers)
			{
				_buffers = new void*[v._nbrBuffers];
				_sizeBuffers = new size_t[v._nbrBuffers];
				for (size_t index = 0; index < _nbrBuffers; ++index)
					_buffers[index] = new uint8_t[v._sizeBuffers[index]];
			}
		}
		memcpy(_sizeBuffers, v._sizeBuffers, sizeof(size_t)* _nbrBuffers);
		for (size_t index = 0; index < _nbrBuffers; ++index)
			memcpy(_sizeBuffers, v._sizeBuffers, sizeof(size_t)* _nbrBuffers);
		_indexOnPool = v._indexOnPool;
		_pool = v._pool;
		return (*this);
	}

	// warning of the index, must be between 0 and index
	void const *Vertices::getBuffer(uint8_t index) const
	{
		if (index >= _nbrBuffers)
		{
			WARNING_MESSAGE_INDEX("getBuffer");
			return (NULL);
		}
		return (_buffers[index]);
	}

	// warning of the index, must be between 0 and index
	size_t Vertices::getSizeBuffer(uint8_t index) const
	{
		if (index >= _nbrBuffers)
		{
			WARNING_MESSAGE_INDEX("getSizeBuffer");
			return (NULL);
		}
		return (_sizeBuffers[index]);
	}

	uint8_t Vertices::getNbrBuffers() const
	{
		return (_nbrBuffers);
	}

	size_t Vertices::getNbrVertices() const
	{
		return (_nbrVertices);
	}

	
}