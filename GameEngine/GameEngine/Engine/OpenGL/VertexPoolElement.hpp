#ifdef VERTEXPOOLELEMENT_HH_

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::VertexPoolElement(uint32_t nbrEntity, Vertice<NBR_ATTRIBUTE> vertices)
: _nbrEntity(nbrEntity),
_vertices(vertices),
_vertexOffset(0),
_byteOffset(0)
{
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy)
: nbrEntity(copy.nbrEntity),
vertices(copy.vertices),
_vertexOffset(copy._vertexOffset),
_byteOffset(copy._byteOffset)
{
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE> &VertexPoolElement<NBR_ATTRIBUTE>::operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element)
{
	_nbrEntity = element._nbrEntity;
	_vertices = element._vertices;
	_vertexOffset = element._vertexOffset;
	_byteOffset = element._byteOffset;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::~VertexPoolElement()
{
}

template <uint8_t NBR_ATTRIBUTE>
bool VertexPoolElement<NBR_ATTRIBUTE>::addVertice(Vertice<NBR_ATTRIBUTE> const &vertices)
{
	if (_nbrEntity == 0)
	{
		_nbrEntity = 1;
		_vertices = vertices;
		return (true);
	}
	if (_vertices == vertices)
	{
		_nbrEntity = _nbrEntity + 1;
		return (true);
	}
	return (false);
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
uint32_t VertexPoolElement<NBR_ATTRIBUTE>::getByteOffset() const
{
	return (_byteOffset);
}

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE> const &VertexPoolElement<NBR_ATTRIBUTE>::getVertex() const
{
	return (_vertices);
}

#endif /*VERTEXMANAGER_HH_*/