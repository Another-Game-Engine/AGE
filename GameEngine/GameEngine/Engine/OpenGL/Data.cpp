#include "Data.hh"

Data::Data(uint32_t nbrByte, uint32_t nbrComponent, uint32_t nbrElement, void *buffer)
: _nbrByte(nbrByte), _nbrComponent(nbrComponent), _nbrElement(nbrElement), _sizeBuffer(nbrByte * nbrComponent * nbrElement)
{
	_buffer = new uint8_t[_sizeBuffer];
	memcpy(_buffer, buffer, _sizeBuffer);
}

Data::Data(Data const &copy)
: _nbrByte(copy._nbrByte), _nbrComponent(copy._nbrComponent), _nbrElement(copy._nbrElement), _sizeBuffer(copy._nbrByte * copy._nbrComponent * copy._nbrElement)
{
	_buffer = new uint8_t[copy._sizeBuffer];
	memcpy(_buffer, copy._buffer, copy._sizeBuffer);
}

Data &Data::operator=(Data const &data)
{
	_nbrByte = data._nbrByte;
	_nbrComponent = data._nbrComponent;
	_nbrElement = data._nbrElement;
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

uint32_t Data::getNbrComponent() const
{
	return (_nbrComponent);
}

uint32_t Data::getNbrByte() const
{
	return (_nbrByte);
}

uint32_t Data::getNbrElement() const
{
	return (_nbrElement);
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
	if (_nbrByte != data._nbrByte || _nbrComponent != data._nbrComponent || _nbrElement != data._nbrElement || _sizeBuffer != data._sizeBuffer)
		return (false);
	if ((memcmp(_buffer, data._buffer, _sizeBuffer)) != 0)
		return (false);
	return (true);
}

bool Data::operator!=(Data const &data) const
{
	return (!(*this == data));
}

void Data::clearBuffer()
{
	if (_buffer)
	{
		delete[] _buffer;
		_buffer = NULL;
	}
}