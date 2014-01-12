#ifdef VERTEXMANAGER_HH_

template <uint16_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::VertexPool()
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
VertexPool<NBR_ATTRIBUTE>::~VertexPool()
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::VertexPool(VertexPool const &copy)
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
VertexPool<NBR_ATTRIBUTE> &VertexPool<NBR_ATTRIBUTE>::operator=(VertexPool<NBR_ATTRIBUTE> const &vertexpool)
{
	_poolElements = vertexpool._poolElements;
	_sizeIndices = vertexpool_sizeIndices;
	_sizeData = vertexpool._sizeData;
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
	{
		_sizeAttribute[index] = vertexpool._sizeAttibute[index];
		_typeAttribute[index] = vertexpool._typeAttribute[index];
		_normalizedAttribute[index] = vertexpool._normalizedAttribute[index];
		_strideAttribute[index] = vertexpool._strideAttribute[index];
		_pointerAttribute[index] = vertexpool._pointerAttribute[index];
	}
	return (*this)
}

template <uint16_t NBR_ATTRIBUTE>
int32_t VertexPool<NBR_ATTRIBUTE>::operator==(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	for (uint32_t index = 0; index < _pool.size(); ++index)
		if (_poolElements[index].vertex == vertex)
			return (index);
	return (-1);
}

template <uint16_t NBR_ATTRIBUTE>
int32_t VertexPool<NBR_ATTRIBUTE>::operator!=(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	return (!(*this == vertex));
}

template <uint16_t NBR_ATTRIBUTE>
bool VertexPool<NBR_ATTRIBUTE>::checkDataVertex(Vertex<NBR_ATTRIBUTE> const &vertex) const
{
	for (uint16_t indexAtt = 0; indexAtt < NBR_ATTRIBUTE; ++indexAtt)
	{
		if (_poolElement.front()[indexAtt].consistency(vertex[indexAtt]) == false)
			return (false);
	}
	return (true);
}

template <uint16_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::setDataAttributeAtStart(Vertex<NBR_ATTRIBUTE> const &vertex)
{
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
	{
		_sizeAttribute[index] = vertex[index].getNbrComponent();
		switch (vertex[index].getNbrByte();)
		{
		case 1:
			_typeAttribute[index] = GL_BYTE;
			break;
		case 2:
			_typeAttribute[index] = GL_SHORT;
			break;
		case 4:
			_typeAttribute[index] = GL_FLOAT;
			break;
		}
	}
}

template <uint16_t NBR_ATTRIBUTE>
int32_t &VertexPool<NBR_ATTRIBUTE>::operator+=(Vertex<NBR_ATTRIBUTE> const &vertex)
{
	if (_poolElement.size() > 0)
	{
		if (checkDataAttribute(vertex) == false)
			return (-1);
	}
	else
		setDataAttributeAtStart(vertex);
	_pointerAttribute[0] = 0;
	for (uint16_t index = 1; index < NBR_ATTRIBUTE; ++index)
		_pointerAttribute[index] += vertex[index - 1].getSizeBuffer();
	/*
	* remplir les offsets de :
	* - Buffer
	* - VertexPointer
	* - Indice
	*/
	_poolElements.push_back(1, vertex);
	_sizeIndices += vertex.getSizeIndices();
	_sizeData += vertex.getSizeAttributes();
	return (_poolElements.size() - 1);
}

template <uint16_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::operator-=(uint32_t index)
{
	if (index < _poolElements.size())
	{
		for (uint16_t index = 1; index < NBR_ATTRIBUTE; ++index)
			_pointerAttribute[index] -= _poolElement[index].getSizeBuffer();
		_sizeIndices -= _poolElements[index].getSizeIndices();
		_sizeData -= _poolElements[index].getSizeAttibutes();
		_poolElements.erase(index);
	}
	else
		std::cerr << "Warning: you try to delete an index superior than the Pool's size" << std::endl;
}

template <uint16_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::clear()
{
	for (size_t index = 0; index < _poolElements.size(); ++index)
	if (!_poolElement[index])
		*this -= index;
}

template <uint16_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::fullClear()
{
	_poolElements.clear();
	_sizeIndices = 0;
	_sizeData = 0;
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_pointerAttribute[index] = 0;
}

template <uint16_t NBR_ATTRIBUTE>
size_t VertexPool<NBR_ATTRIBUTE>::size() const
{
	return (_poolElements.size());
}

template <uint16_t NBR_ATTRIBUTE>
GLint VertexPool<NBR_ATTRIBUTE>::sizeAttribute(GLint index) const
{
	return (_sizeAttibute[index]);
}

template <uint16_t NBR_ATTRIBUTE>
GLenum VertexPool<NBR_ATTRIBUTE>::typeAttribute(GLint index) const
{
	return (_typeAttribute[index]);
}

template <uint16_t NBR_ATTRIBUTE>
GLenum VertexPool<NBR_ATTRIBUTE>::normalizedAttribute(GLint index) const
{
	return (_normalizedAttribute[index]);
}

template <uint16_t NBR_ATTRIBUTE>
GLsizei VertexPool<NBR_ATTRIBUTE>::strideAttribute(GLint index) const
{
	return (_strideAttribute[index]);
}

template <uint16_t NBR_ATTRIBUTE>
GLuint VertexPool<NBR_ATTRIBUTE>::pointerAttribute(GLint index) const
{
	return (_pointerAttribute[index]);
}

#endif /*!VERTEXMANAGER_HH_*/