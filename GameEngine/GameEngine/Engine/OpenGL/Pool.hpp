#ifdef VERTEXMANAGER_HH_

/**
* - Declaration of the struct into VertexManager.hh
* - implement PoolElement and Pool struct use by VertexManager.hh
*/

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::Pool::Pool()
: _sizeIndices(0),
_sizeData(0)
{
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
	{
		_sizeAttribute[index] = 0;
		_typeAttribute[index] = 0;
		_normalizedAttribute[index] = GL_FALSE;
		_strideAttribute[index] = 0;
		_pointerAttribute[index] = 0;
	}
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::Pool::~Pool()
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::Pool::Pool(Pool const &copy)
: _poolElements(copy._poolElements)
_sizeIndices(copy._sizeIndices),
_sizeData(copy._sizeData)
{
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
	{
		_sizeAttribute[index] = copy._sizeAttibute[index];
		_typeAttribute[index] = copy._typeAttribute[index];
		_normalizedAttribute[index] = copy._normalizedAttribute[index];
		_strideAttribute[index] = copy._strideAttribute[index];
		_pointerAttribute[index] = copy._pointerAttribute[index];
	}
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::Pool &VertexManager<NBR_ATTRIBUTE>::Pool::operator=(Pool const &pool)
{
	_poolElements = pool._poolElements;
	_sizeIndices = pool._sizeIndices;
	_sizeData = pool._sizeData;
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
	{
		_sizeAttribute[index] = pool._sizeAttibute[index];
		_typeAttribute[index] = pool._typeAttribute[index];
		_normalizedAttribute[index] = pool._normalizedAttribute[index];
		_strideAttribute[index] = pool._strideAttribute[index];
		_pointerAttribute[index] = pool._pointerAttribute[index];
	}
	return (*this)
}

template <uint16_t NBR_ATTRIBUTE>
int32_t VertexManager<NBR_ATTRIBUTE>::Pool::operator==(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	for (uint32_t index = 0; index < _pool.size(); ++index)
		if (_poolElements[index].vertex == vertex)
			return (index);
	return (-1);
}

template <uint16_t NBR_ATTRIBUTE>
int32_t VertexManager<NBR_ATTRIBUTE>::Pool::operator!=(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	return (!(*this == vertex));
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::Pool::checkDataVertex(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	for (uint16_t indexAtt = 0; indexAtt < NBR_ATTRIBUTE; ++indexAtt)
	{
		if (_poolElement.front()[indexAtt].consistency(vertex[indexAtt]) == false)
			return (false);
	}
	return (true);
}

template <uint16_t NBR_ATTRIBUTE>
int32_t &VertexManager<NBR_ATTRIBUTE>::Pool::operator+=(Vertex<NBR_ATTRIBUTE> const &vertex)
{
	if (_startPool == false)
	{
		_startPool = true;
	}
	else
	{
		if (checkDataAttribute(vertex) == false)
		{
			_startPool = false;
			return (-1);
		}
	}
	_poolElements.push_back(1, vertex);
	_sizeIndices += vertex.getSizeIndices();
	_sizeData += vertex.getSizeAttributes();
	return (_poolElements.size() - 1);
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::Pool::clear()
{
	_poolElements.clear();
	_sizeIndices = 0;
	_sizeData = 0;
}

template <uint16_t NBR_ATTRIBUTE>
size_t VertexManager<NBR_ATTRIBUTE>::Pool::size() const
{
	return (_poolElements.size());
}

template <uint16_t NBR_ATTRIBUTE>
GLint _VertexManager<NBR_ATTRIBUTE>::Pool::sizeAttribute(GLint index) const
{

}

#endif /*!VERTEXMANAGER_HH_*/