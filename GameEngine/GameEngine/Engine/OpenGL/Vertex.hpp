#ifdef VERTEX_HH_

template <uint32_t NBR_ATTRIBUTE>
Vertex<NBR_ATTRIBUTE>::Vertex(std::array<Data, NBR_ATTRIBUTE> attributes, Data const * const indices)
: _attributes(attributes),
	_sizeIndices(0),
	_vertexManager(NULL),
	_indexPoolVertexManager(-1)
{
	for (uint32_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_sizeAttributes += _attribute[index].getSizeBuffer();
	if (indices)
	{
		_indices = new Data(indices);
		_sizeIndices = _indices->getSizeBuffer();
	}
}

template <uint32_t NBR_ATTRIBUTE>
Vertex<NBR_ATTRIBUTE>::Vertex(Vertex<NBR_ATTRIBUTE> const &copy)
: _attributes(copy._attributes),
	_size(copy._size),
	_vertexManager(copy._vertexManager),
	_indexPoolVertexManager(copy._indexPoolVertexManager)
{
	if (copy.indices)
	{
		_indices = new Data(copy.indices);
	}
}

template <uint32_t NBR_ATTRIBUTE>
Vertex<NBR_ATTRIBUTE>::~Vertex()
{
	if (_indices)
		delete _indices;
}

template <uint32_t NBR_ATTRIBUTE>
Vertex<NBR_ATTRIBUTE> &Vertex<NBR_ATTRIBUTE>::operator=(Vertex<NBR_ATTRIBUTE> const &vertex)
{
	_attributes = vertex._attributes;
	if (vertex._indices)
		_indices = new Data(vertex.indices);
	_vertexManager = vertex._vertexManager;
	_indexPoolVertexManager = vertex._indexPool;
	return (*this);
}

template <uint32_t NBR_ATTRIBUTE>
Data const &Vertex<NBR_ATTRIBUTE>::operator[](size_t index) const
{
	return (_attributes[index]);
}

template <uint32_t NBR_ATTRIBUTE>
Data const * const Vertex<NBR_ATTRIBUTE>::getIndices() const
{
	return (_indices);
}

template <uint32_t NBR_ATTRIBUTE>
bool Vertex<NBR_ATTRIBUTE>::operator==(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	if (vertex._indices && _indices)
	{
		if (vertex._indices != _indices)
			return (false);
	}
	for (uint32_t index = 0; index < NBR_ATTRIBUTE; ++index)
	if (_attributes[index] == vertex._attributes[index])
		return (false);
	return (true);
}

template <uint32_t NBR_ATTRIBUTE>
bool Vertex<NBR_ATTRIBUTE>::operator!=(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	return (!(*this == vertex));
}

template <uint32_t NBR_ATTRIBUTE>
uint32_t Vertex<NBR_ATTRIBUTE>::getSizeAttributes() const
{
	return (_sizeAttributes);
}

template <uint32_t NBR_ATTRIBUTE>
uint32_t Vertex<NBR_ATTRIBUTE>::getSizeIndices() const
{
	return (_sizeIndices);
}

template <uint32_t NBR_ATTRIBUTE>
uint32_t Vertex<NBR_ATTRIBUTE>::draw() const
{
	if (_vertexManager != NULL)
	{

	}
}

template <uint32_t NBR_ATTRIBUTE>
bool Vertex<NBR_ATTRIBUTE>::isDrawable() const
{
	if (_vertexManager == NULL)
		return (false);
	else
		return (true);
}

#endif /*!VERTEX_HPP_*/