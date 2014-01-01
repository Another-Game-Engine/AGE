#ifdef VERTEXMANAGER_HH_

/**
* - Declaration of the struct into VertexManager.hh
* - implement PoolElement and Pool struct use by VertexManager.hh
*/

/**
* Implementation of Pool
*/

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::Pool::Pool()
: sizeIndices(0),
sizeData(0)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::Pool::~Pool()
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::Pool::Pool(Pool const &copy)
: poolElements(copy.poolElements)
sizeIndices(copy.sizeIndices),
sizeData(copy.sizeData)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::Pool &VertexManager<NBR_ATTRIBUTE>::Pool::operator=(Pool const &pool)
{
	oolElements = pool.poolElements;
	sizeIndices = pool.sizeIndices;
	sizeData = pool.sizeData;
	return (*this)
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::Pool::operator==(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	for (uint32_t index = 0; index < _pool.size(); ++index)
		if (poolElements[index].vertex == vertex)
			return (true);
	return (false);
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::Pool::operator!=(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	return (!(*this == vertex));
}

template <uint16_t NBR_ATTRIBUTE>
Pool &VertexManager<NBR_ATTRIBUTE>::Pool::operator+=(Vertex<NBR_ATTRIBUTE> const &vertex)
{
	poolElements.push_back(1, vertex);
	sizeIndices += vertex.getSizeIndices();
	sizeData += vertex.getSizeAttributes();
	return (*this);
}

/**
* Implementation of PoolElement
*/

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::PoolElement::PoolElement(uint32_t nbrEntity, Vertex<NBR_ATTRIBUTE> vertex)
: nbrEntity(nbrEntity),
	vertex(vertex)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::PoolElement::PoolElement(VertexManager<NBR_ATTRIBUTE>::PoolElement const &copy)
: nbrEntity(copy.nbrEntity),
vertex(copy.vertex)
{
	
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::PoolElement &VertexManager<NBR_ATTRIBUTE>::operator=(VertexManager<NBR_ATTRIBUTE>::PoolElement const &element)
{
	nbrEntity = element.nbrEntity;
	vertex = element.vertex;
	return (*this);
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::PoolElement::~PoolElement()
{
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::PoolElement::operator==(VertexManager<NBR_ATTRIBUTE>::PoolElement const &element)
{
	return (vertex == element.vertex);
}
template <uint16_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::PoolElement::operator==(VertexManager<NBR_ATTRIBUTE>::PoolElement const &element)
{
	return (vertex == element.vertex);
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::PoolElement::operator!=(VertexManager<NBR_ATTRIBUTE>::PoolElement const &element)
{
	return (vertex != element.vertex);
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::PoolElement::operator==(Vertex<NBR_ATTRIBUTE> const &element)
{
	return (vertex == element);
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::PoolElement::operator!=(Vertex<NBR_ATTRIBUTE> const &element)
{
	return (vertex != element);
}

#endif /*!VERTEXMANAGER_HH_*/