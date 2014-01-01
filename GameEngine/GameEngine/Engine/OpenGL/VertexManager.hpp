#ifdef VERTEXMANAGER_HH_

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager()
{
}


template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager()
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager(VertexManager<NBR_ATTRIBUTE> const &copy)
: _vertexArray(copy._vertexArray),
_indexBuffer(copy._indexBuffer),
_dataBuffer(copy._dataBuffer)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE> &VertexManager<NBR_ATTRIBUTE>::operator=(VertexManager<NBR_ATTRIBUTE> const &vertexmanager)
{
	_vertexArray = vertexmanager._vertexArray;
	_indiceBuffer = vertexmanager._indexBuffer;
	_dataBuffer = vertexmanager._dataBuffer;
	return (*this);
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::addVertex(Vertex<NBR_ATTRIBUTE> const &vertex, std::string const &name)
{
	if (_pool != vertex)
		_pool += vertex;
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::deleteVertex(std::string const &name)
{
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::clear()
{
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::draw(GLenum mode)
{
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::sendToGraphicProcessUnit(GLenum mode) const
{
}

#endif /*VERTEXMANAGER_HH_*/