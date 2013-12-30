#include "Vertex.hh"

VertexData::VertexData(uint32_t nbrByte, uint32_t nbrComponent, uint32_t nbrElement, void *buffer)
: _nbrByte(nbrByte), _nbrComponent(nbrComponent), _nbrElement(nbrElement), _sizeBuffer(nbrByte * nbrComponent * nbrElement)
{
	static uint32_t id = 0;
	_id = id;
	++id;
	_buffer = new uint8_t[_sizeBuffer];
	memcpy(_buffer, buffer, _sizeBuffer);
}

VertexData::VertexData(VertexData const &copy)
: _id(copy._id), _nbrByte(copy._nbrByte), _nbrComponent(copy._nbrComponent), _nbrElement(copy._nbrElement), _sizeBuffer(copy._nbrByte * copy._nbrComponent * copy._nbrElement)
{
	_buffer = new uint8_t[copy._sizeBuffer];
	memcpy(_buffer, copy._buffer, copy._sizeBuffer);
}

VertexData &VertexData::operator=(VertexData const &vertexdata)
{
	_id = vertexdata._id;
	_nbrByte = vertexdata._nbrByte;
	_nbrComponent = vertexdata._nbrComponent;
	_nbrElement = vertexdata._nbrElement;
	_buffer = new uint8_t[vertexdata._sizeBuffer];
	memcpy(_buffer, vertexdata._buffer, vertexdata._sizeBuffer);
	_sizeBuffer = vertexdata._sizeBuffer;
	return (*this);
}

VertexData::~VertexData()
{
	if (_buffer)
		delete[] _buffer;
}

uint32_t VertexData::getNbrComponent() const
{
	return (_nbrComponent);
}

uint32_t VertexData::getNbrByte() const
{
	return (_nbrByte);
}

uint32_t VertexData::getNbrElement() const
{
	return (_nbrElement);
}

uint32_t VertexData::getSizeBuffer() const
{
	return (_sizeBuffer);
}

void const * const VertexData::getBuffer() const
{
	return (_buffer);
}

bool VertexData::operator==(VertexData const &vertexData) const
{
	return (_id == vertexData._id);
}

bool VertexData::operator!=(VertexData const &vertexData) const
{
	return (!(*this == vertexData));
}

void VertexData::clearBuffer()
{
	if (_buffer)
	{
		delete[] _buffer;
		_buffer = NULL;
	}
}