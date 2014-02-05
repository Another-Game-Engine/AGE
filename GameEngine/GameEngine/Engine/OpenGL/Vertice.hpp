#ifdef VERTICE_HH_

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE>::Vertice(uint32_t nbrVertex, std::array<Data, NBR_ATTRIBUTE> attribData, Data const * const indices)
: _bufferData(attribData),
_sizeVertexBuffer(0),
_sizeIndicesBuffer(0),
_nbrVertex(nbrVertex),
_nbrIndices(0),
_indices(NULL),
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
Vertice<NBR_ATTRIBUTE>::Vertice()
: _sizeVertexBuffer(0),
_sizeIndicesBuffer(0),
_nbrVertex(0),
_nbrIndices(0),
_indices(NULL),
_vertexManager(NULL),
_index(-1)
{
}

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE>::Vertice(Vertice<NBR_ATTRIBUTE> const &copy)
: _bufferData(copy._bufferData),
_sizeVertexBuffer(copy._sizeVertexBuffer),
_sizeIndicesBuffer(copy._sizeIndicesBuffer),
_nbrVertex(copy._nbrVertex),
_nbrIndices(copy._nbrIndices),
_indices(copy._indices),
_vertexManager(copy._vertexManager),
_index(copy._index)
{
	if (copy._indices)
		_indices = new Data(*copy._indices);
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
	_bufferData = vertex._bufferData;
	_sizeVertexBuffer = vertex._sizeVertexBuffer;
	_sizeIndicesBuffer = vertex._sizeIndicesBuffer;
	_nbrVertex = vertex._nbrVertex;
	_nbrIndices = vertex._nbrIndices;
	if (vertex._indices)
		_indices = new Data(*vertex._indices);
	else
		_indices = NULL;
	if (_index != -1 && _vertexManager != NULL)
		_vertexManager->deleteVertice(*this);
	_vertexManager = vertex._vertexManager;
	_index = vertex._index;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
bool Vertice<NBR_ATTRIBUTE>::operator==(Vertice<NBR_ATTRIBUTE> const &vertex) const
{
	if (vertex._indices && _indices)
	{
		if (*vertex._indices != *_indices)
			return (false);
	}
	for (uint8_t index = 0; index < NBR_ATTRIBUTE; ++index)
	{
		if (_bufferData[index] != vertex._bufferData[index])
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
void const * const Vertice<NBR_ATTRIBUTE>::getBuffer(uint8_t index) const
{
	return (_bufferData[index].getBuffer());
}

template <uint8_t NBR_ATTRIBUTE>
void const * const Vertice<NBR_ATTRIBUTE>::getIndices() const
{
	if (_indices)
		return (_indices->getBuffer());
	else
		return (NULL);
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
void Vertice<NBR_ATTRIBUTE>::draw(GLenum mode) const
{
	if (_vertexManager != NULL)
	{
		_vertexManager->callDraw(this, mode);
	}
}

#endif /*!VERTEX_HPP_*/