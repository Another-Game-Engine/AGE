#ifdef VERTICE_HH_

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE>::Vertice(uint32_t nbrVertex, std::array<Data, NBR_ATTRIBUTE> attribData, Data const * const indices)
: _bufferData(attribData),
_sizeVertexBuffer(0),
_sizeIndicesBuffer(0),
_nbrVertex(nbrVertex),
_nbrIndices(0),
_vertexManager(NULL),
_index(-1)
{
	for (uint8_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_sizeVertexBuffer += _bufferData[index].getSizeBuffer();
	if (indices != NULL)
	{
		_indices = new Data(*indices);
		_sizeIndicesBuffer = _indices->getSizeBuffer();
		_nbrIndices = _indices->getSizeBuffer() / sizeof(unsigned int);
	}
}

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE>::Vertice(Vertice<NBR_ATTRIBUTE> const &copy)
: _bufferData(copy._bufferData),
_sizeVertexBuffer(copy._sizeVertexBuffer),
_sizeIndicesBuffer(copy._sizeIndicesBuffer),
_nbrVertex(copy._nbrVertex),
_nbrIndice(copy._nbrIndice),
_vertexManager(copy._vertexManager),
_index(copy._index)
{
	if (copy.indices)
		_indices = new Data(copy.indices);
}

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE>::~Vertice()
{
	if (_indices)
		delete _indices;
}

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE> &Vertice<NBR_ATTRIBUTE>::operator=(Vertice<NBR_ATTRIBUTE> const &vertex)
{
	_bufferData = vertex._bufferDatas;
	_attribComponent = vertex._attribComponent;
	_attribByte = vertex._attribByte;
	_sizeVertexBuffer = vertex._sizeVertexBuffer;
	_sizeIndicesBuffer = vertex._sizeIndicesBuffer;
	_nbrVertex = vertex._nbrVertex;
	_nbrIndice = vertex._nbrIndice;
	if (vertex._indices)
		_indices = new Data(vertex.indices);
	_vertexManager = vertex._vertexManager;
	_index = vertex._indexPool;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
bool Vertice<NBR_ATTRIBUTE>::operator==(Vertice<NBR_ATTRIBUTE> const &vertex) const
{
	if (vertex._indices && _indices)
	{
		if (vertex._indices != _indices)
			return (false);
	}
	for (uint32_t index = 0; index < NBR_ATTRIBUTE; ++index)
	{
		if (_bufferData[index] == vertex._bufferData[index])
			return (false);
	}
	return (true);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t Vertice<NBR_ATTRIBUTE>::getSizeVertexBuffer() const
{
	return (_sizeVertexBuffer);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t Vertice<NBR_ATTRIBUTE>::getSizeIndicesBuffer() const
{
	return (_sizeIndicesBuffer);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t Vertice<NBR_ATTRIBUTE>::getNbrVertex() const
{
	return (_nbrVertex);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t Vertice<NBR_ATTRIBUTE>::getNbrIndices() const
{
	return (_nbrIndices);
}

template <uint8_t NBR_ATTRIBUTE>
int32_t Vertice<NBR_ATTRIBUTE>::getIndexPool() const
{
	return (_index);
}

template <uint8_t NBR_ATTRIBUTE>
bool Vertice<NBR_ATTRIBUTE>::isDrawable() const
{
	if (_vertexManager == NULL || _index == -1)
		return (false);
	else
		return (true);
}

template <uint8_t NBR_ATTRIBUTE>
bool Vertice<NBR_ATTRIBUTE>::hasIndices() const
{
	if (_indices == NULL)
		return (false);
	else
		return (true);
}

template <uint8_t NBR_ATTRIBUTE>
void Vertice<NBR_ATTRIBUTE>::draw() const
{
	if (_vertexManager != NULL)
	{
		std::cout << "I'am draw somthing" << std::endl;
	}
}

#endif /*!VERTEX_HPP_*/