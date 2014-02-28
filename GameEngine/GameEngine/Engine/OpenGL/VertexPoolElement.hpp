#ifdef VERTEXPOOLELEMENT_HH_

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::VertexPoolElement(uint32_t nbrEntity, Vertice<NBR_ATTRIBUTE> const &vertices)
: _nbrEntity(nbrEntity),
_vertices(vertices),
_vertexOffset(0),
_indicesOffset(0)
{
	for (uint8_t index = 0; index < NBR_ATTRIBUTE; index++)
	{
		_byteOffset[index] = 0;
		_nbrByte[index] = 0;
	}
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy)
: _nbrEntity(copy._nbrEntity),
_vertices(copy._vertices),
_vertexOffset(copy._vertexOffset),
_indicesOffset(copy._indicesOffset),
_byteOffset(copy._byteOffset),
_nbrByte(copy._nbrByte)
{
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE> &VertexPoolElement<NBR_ATTRIBUTE>::operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element)
{
	_nbrEntity = element._nbrEntity;
	_vertices = element._vertices;
	_vertexOffset = element._vertexOffset;
	_indicesOffset = element._indicesOffset;
	_byteOffset = element._byteOffset,
	_nbrByte = element._nbrByte;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::~VertexPoolElement()
{
}

template <uint8_t NBR_ATTRIBUTE>
AddVerticesResult VertexPoolElement<NBR_ATTRIBUTE>::addVertice(Vertice<NBR_ATTRIBUTE> const &vertices)
{
	if (_nbrEntity == 0)
	{
		_nbrEntity = 1;
		_vertices = vertices;
		return (SET);
	}
	if (_vertices == vertices)
	{
		_nbrEntity = _nbrEntity + 1;
		return (ADD);
	}
	return (FAIL);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPoolElement<NBR_ATTRIBUTE>::deleteVertice()
{
	if (_nbrEntity != 0)
		_nbrEntity = _nbrEntity - 1;
}

template <uint8_t NBR_ATTRIBUTE>
bool VertexPoolElement<NBR_ATTRIBUTE>::isEmpty() const
{
	if (_nbrEntity == 0)
		return (true);
	return (false);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPoolElement<NBR_ATTRIBUTE>::getVertexOffset() const
{
	return (_vertexOffset);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPoolElement<NBR_ATTRIBUTE>::getByteOffset(uint8_t index) const
{
	return (_byteOffset[index]);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPoolElement<NBR_ATTRIBUTE>::getNbrByte(uint8_t index) const
{
	return (_nbrByte[index]);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPoolElement<NBR_ATTRIBUTE>::getIndicesOffset() const
{
	return (_indicesOffset);
}

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE> const &VertexPoolElement<NBR_ATTRIBUTE>::getVertex() const
{
	return (_vertices);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPoolElement<NBR_ATTRIBUTE>::settingOffset(uint32_t vertexOffset, uint32_t indicesOffset, std::array<uint32_t, NBR_ATTRIBUTE> const &byteOffset, std::array<uint32_t, NBR_ATTRIBUTE> const &nbrByte)
{
	_vertexOffset = vertexOffset;
	_indicesOffset = indicesOffset;
	_byteOffset = byteOffset;
	_nbrByte = nbrByte;
}

#endif /*VERTEXMANAGER_HH_*/