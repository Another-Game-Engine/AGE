#ifdef VERTEXMANAGER_HH_

/**
* - Declaration of the struct into VertexManager.hh
* - implement PoolElement and Pool struct use by VertexManager.hh
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

template <uint16_t NBR_ATTRIBUTE>
Data<NBR_ATTRIBUTE> const & VertexManager<NBR_ATTRIBUTE>::PoolElement::operator[](size_t index) const
{
	return (vertex[index]);
}

#endif /*VERTEXMANAGER_HH_*/