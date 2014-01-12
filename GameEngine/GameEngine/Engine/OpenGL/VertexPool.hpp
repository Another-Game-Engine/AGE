#ifdef VERTEXMANAGER_HH_

template <uint16_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::VertexPool()
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::~VertexPool()
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::VertexPool(VertexPool const &copy)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE> &VertexPool<NBR_ATTRIBUTE>::operator=(VertexPool<NBR_ATTRIBUTE> const &vertexpool)
{
	return (*this);
}


template <uint16_t NBR_ATTRIBUTE>
int32_t VertexPool<NBR_ATTRIBUTE>::addElement(Vertice<NBR_ATTRIBUTE> const &vertex)
{
	return (0);
}

template <uint16_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::deleteElement(Vertice<NBR_ATTRIBUTE> const &vertex)
{
}

template <uint16_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::clear()
{
}

template <uint16_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::fullClear()
{
}

template <uint16_t NBR_ATTRIBUTE>
size_t VertexPool<NBR_ATTRIBUTE>::getNbrElement() const
{
	return (0);
}

template <uint16_t NBR_ATTRIBUTE>
GLuint VertexPool<NBR_ATTRIBUTE>::getPointerAttribute(GLint index) const
{
	return (0);
}

#endif /*!VERTEXMANAGER_HH_*/