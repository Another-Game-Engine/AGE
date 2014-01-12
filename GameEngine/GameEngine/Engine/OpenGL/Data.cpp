#include "Data.hh"

Data::Data(uint32_t sizeBuffer, void *buffer)
: _sizeBuffer(sizeBuffer)
{
	_buffer = new uint8_t[_sizeBuffer];
	memcpy(_buffer, buffer, _sizeBuffer);
}

Data::Data(Data const &copy)
: _sizeBuffer(copy._sizeBuffer)
{
	_buffer = new uint8_t[copy._sizeBuffer];
	memcpy(_buffer, copy._buffer, copy._sizeBuffer);
}

Data &Data::operator=(Data const &data)
{
	_buffer = new uint8_t[data._sizeBuffer];
	memcpy(_buffer, data._buffer, data._sizeBuffer);
	_sizeBuffer = data._sizeBuffer;
	return (*this);
}

Data::~Data()
{
	if (_buffer)
		delete[] _buffer;
}

uint32_t Data::getSizeBuffer() const
{
	return (_sizeBuffer);
}

void const * const Data::getBuffer() const
{
	return (_buffer);
}

bool Data::operator==(Data const &data) const
{
	if (_sizeBuffer != data._sizeBuffer)
		return (false);
	if ((memcmp(_buffer, data._buffer, _sizeBuffer)) != 0)
		return (false);
	return (true);
}

bool Data::operator!=(Data const &data) const
{
	return (!(*this == data));
}

Data &Data::operator()(uint32_t sizeBuffer, void *buffer)
{
	_sizeBuffer = sizeBuffer;
	if (_buffer)
	{
		delete[] _buffer;
		_buffer = NULL;
	}
	else
	{
		_buffer = new uint8_t[sizeBuffer];
		memcpy(_buffer, buffer, sizeBuffer);
	}
	return (*this);
}