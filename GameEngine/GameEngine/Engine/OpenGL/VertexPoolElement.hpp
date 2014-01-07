#ifdef VERTEXPOOLELEMENT_HH_

/**
* - Declaration of the struct into VertexManager.hh
* - implement PoolElement and Pool struct use by VertexManager.hh
*/

template <uint16_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::VertexPoolElement(uint32_t nbrEntity, Vertex<NBR_ATTRIBUTE> vertex)
: nbrEntity(nbrEntity),
vertex(vertex)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy)
: nbrEntity(copy.nbrEntity),
vertex(copy.vertex)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE> &VertexPoolElement<NBR_ATTRIBUTE>::operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element)
{
	nbrEntity = element.nbrEntity;
	vertex = element.vertex;
	return (*this);
}

template <uint16_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::~VertexPoolElement()
{
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexPoolElement<NBR_ATTRIBUTE>::operator==(VertexPoolElement<NBR_ATTRIBUTE> const &element)
{
	return (vertex == element.vertex);
}
template <uint16_t NBR_ATTRIBUTE>
bool VertexPoolElement<NBR_ATTRIBUTE>::operator==(VertexPoolElement<NBR_ATTRIBUTE> const &element)
{
	return (vertex == element.vertex);
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexPoolElement<NBR_ATTRIBUTE>::operator!=(VertexPoolElement<NBR_ATTRIBUTE> const &element)
{
	return (vertex != element.vertex);
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexPoolElement<NBR_ATTRIBUTE>::operator==(Vertex<NBR_ATTRIBUTE> const &element)
{
	return (vertex == element);
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexPoolElement<NBR_ATTRIBUTE>::operator!=(Vertex<NBR_ATTRIBUTE> const &element)
{
	return (vertex != element);
}

template <uint16_t NBR_ATTRIBUTE>
Data<NBR_ATTRIBUTE> const & VertexPoolElement<NBR_ATTRIBUTE>::operator[](size_t index) const
{
	return (vertex[index]);
}

#endif /*VERTEXMANAGER_HH_*/