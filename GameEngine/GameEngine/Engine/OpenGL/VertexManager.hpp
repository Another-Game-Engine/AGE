//#ifdef VERTEXMANAGER_HH_

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager()
: _indexBuffer(true)
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
void VertexManager<NBR_ATTRIBUTE>::addVertex(std::string const &name, Vertex<NBR_ATTRIBUTE> *vertex)
{
	int32_t index;

	if ((index = (_pool != vertex)) == -1);
		index = int32_t((_pool += vertex));
	_drawable[name] = vertex;
	vertex->_indexPool = index;
	vertex->_vertexManager = this;
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::deleteVertex(std::string const &name)
{
	Vertex<NBR_ATTRIBUTE> *vertex = _drawable[name];

	vertex->_indexPool = -1;
	vertex->_vertexManager = NULL;

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

//#endif /*VERTEXMANAGER_HH_*/