#include <Render/Data.hh>
#include <iostream>
#include <string>
#include <cassert>

namespace gl
{
	Data::Data()
		: _nbrElement(0)
	{
	}

	Data::Data(size_t nbrElement, AGE::Vector<size_t> const &sizeBuffers, AGE::Vector<void *> const &buffers)
		: _buffers(buffers.size()),
		_sizeBuffers(sizeBuffers),
		_nbrElement(nbrElement)
	{
		copyData(buffers, sizeBuffers);
	}

	Data::Data(size_t nbrElement, AGE::Vector<uint32_t> const &buffer)
		: _buffers({ new uint32_t[nbrElement] }),
		_nbrElement(nbrElement)
	{
		_sizeBuffers.push_back(sizeof(uint32_t)* nbrElement);
		for (size_t index = 0; index < _sizeBuffers.back(); ++index)
		{
			((uint8_t *)_buffers.back())[index] = ((uint8_t *)buffer.data())[index];
		}
	}

	Data::Data(Data const &copy)
		: _buffers(copy._buffers.size()),
		_sizeBuffers(copy._sizeBuffers),
		_nbrElement(copy._nbrElement)
	{
		copyData(copy._buffers, copy._sizeBuffers);
	}

	Data::~Data()
	{
		cleanData();
	}

	Data &Data::operator=(Data const &v)
	{
		cleanData();
		_sizeBuffers = v._sizeBuffers;
		_nbrElement = v._nbrElement;
		_buffers.resize(v._buffers.size());
		copyData(v._buffers, v._sizeBuffers);
		return (*this);
	}

	// warning of the index, must be between 0 and index
	void const *Data::getBuffer(uint8_t index) const
	{
		return (_buffers[index]);
	}

	// warning of the index, must be between 0 and _nbrBuffers
	size_t Data::getSizeBuffer(uint8_t index) const
	{
		return (_sizeBuffers[index]);
	}

	size_t Data::getNbrBuffers() const
	{
		return (_sizeBuffers.size());
	}

	size_t Data::getNbrElement() const
	{
		return (_nbrElement);
	}
	
	void Data::copyData(AGE::Vector<void *> const &data, AGE::Vector<size_t> const &sizeData)
	{
		for (size_t b = 0; b < _buffers.size(); ++b)
		{
			_buffers[b] = new uint8_t[sizeData[b]];
			for (size_t index = 0; index < sizeData[b]; ++index)
			{
				((uint8_t *)_buffers[b])[index] = ((uint8_t *)data[b])[index];
			}
		}
	}

	void Data::cleanData()
	{
		for (size_t index = 0; index < _buffers.size(); ++index)
		{
			if (_buffers[index] != NULL)
			{
				delete[] _buffers[index];
				_buffers[index] = NULL;
			}
		}
	}
}
