#include <Render/Data.hh>
#include <iostream>
#include <string>
#include <cassert>

namespace gl
{
	Vertices::Vertices()
		: _buffers(NULL),
		_sizeBuffers(NULL),
		_nbrVertices(0),
		_nbrBuffers(0)
	{
	}

	Vertices::Vertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers)
		: _buffers(NULL),
		_sizeBuffers(NULL),
		_nbrVertices(nbrVertices),
		_nbrBuffers(nbrBuffers)
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
		_nbrBuffers(copy._nbrBuffers)
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
			memcpy(_buffers[index], v._buffers[index], _sizeBuffers[index]);
		return (*this);
	}

	// warning of the index, must be between 0 and index
	void const *Vertices::getBuffer(uint8_t index) const
	{
		if (index >= _nbrBuffers)
			assert(0);
		return (_buffers[index]);
	}

	// warning of the index, must be between 0 and _nbrBuffers
	size_t Vertices::getSizeBuffer(uint8_t index) const
	{
		if (index >= _nbrBuffers)
			assert(0);
		return (_sizeBuffers[index]);
	}

	uint8_t Vertices::getNbrBuffers() const
	{
		return (_nbrBuffers);
	}

	size_t Vertices::getNbrElement() const
	{
		return (_nbrVertices);
	}

	Indices::Indices()
		: _nbrIndices(0),
		_buffer(NULL)
	{

	}

	Indices::Indices(size_t nbrIndices, uint32_t *buffer)
		: _nbrIndices(nbrIndices),
		_buffer(NULL)
	{
		if (buffer)
		{
			_buffer = new uint32_t[nbrIndices];
			memcpy(_buffer, buffer, sizeof(uint32_t)* nbrIndices);
		}
	}

	Indices::Indices(Indices const &copy)
		: _nbrIndices(copy._nbrIndices),
		_buffer(NULL)
	{
		if (copy._buffer)
		{
			_buffer = new uint32_t[_nbrIndices];
			memcpy(_buffer, copy._buffer, sizeof(uint32_t) * _nbrIndices);
		}
	}

	Indices &Indices::operator=(Indices const &i)
	{
		if (this != &i)
		{
			_nbrIndices = i._nbrIndices;
			if (i._buffer)
			{
				_buffer = new uint32_t[_nbrIndices];
				memcpy(_buffer, i._buffer, sizeof(uint32_t)* _nbrIndices);
			}
		}
		return (*this);
	}

	size_t Indices::getNbrElement() const
	{
		return (_nbrIndices);
	}

	uint32_t const *Indices::getBuffer(size_t) const
	{
		return (_buffer);
	}
	
}
