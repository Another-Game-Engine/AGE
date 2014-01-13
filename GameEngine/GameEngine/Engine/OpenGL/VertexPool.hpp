#ifdef VERTEXPOOL_HH_

template <uint8_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::VertexPool()
: _sizeIndices(0),
_sizeBuffer(0),
_updateMajor(false),
_updateMinor(false)
{
	for (uint8_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_pointerAttributes[index] = 0;
}

template <uint8_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::~VertexPool()
{
}

template <uint8_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::VertexPool(VertexPool<NBR_ATTRIBUTE> const &copy)
: _elements(copy._elements),
_sizeIndices(copy._sizeIndices),
_sizeBuffer(copy._sizeBuffer),
_pointerAttributes(copy._pointerAttributes),
_updateBuffer(copy._updateBuffer),
_updateMajor(copy._updateMajor),
_updateMinor(copy._updateMinor)
{
}

template <uint8_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE> &VertexPool<NBR_ATTRIBUTE>::operator=(VertexPool<NBR_ATTRIBUTE> const &vertexpool)
{
	_elements = vertexpool._elements;
	_sizeIndices = vertexpool._sizeIndices;
	_sizeBuffer = vertexpool._sizeBuffer;
	_pointerAttributes = vertexpool._pointerAttributes;
	_updateBuffer = vertexpool._updateBuffer;
	_updateMajor = vertexpool._updateMajor;
	_updateMinor = vertexpool._updateMinor;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE> const &VertexPool<NBR_ATTRIBUTE>::operator[](uint32_t index)
{
	return (_element[index]);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPool<NBR_ATTRIBUTE>::addElement(Vertice<NBR_ATTRIBUTE> const &vertices)
{
	uint32_t sizeNotUse = 0;

	for (uint32_t index = 0; index < _element.size(); ++index)
	{
		if ((AddVerticeResult ret = _elements[index].addVertice(vertices)) != FAIL)
		{
			if (ret == SET)
			{
				_updateMinor = true;
				_updateBuffer.push(index);
			}
			return (index);
		}
		if (_elements[index].isEmpty())
			sizeNotUse += _elements[index].getVertex().getSizeBuffer();
	}
	if (_sizeBuffer > 0 && (float(sizeNotUse) / float(_sizeBuffer)) > 0.75)
		clear();
	_elements.push_back(VertexPoolElement(1, vertex));
	_sizeIndices += vertex.getSizeIndices();
	_sizeBuffer += vertex.getSizeBuffer();
	_updateMajor = true;
	return (_elements.size() - 1);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::deleteElement(Vertice<NBR_ATTRIBUTE> const &vertex)
{
	if (vertex.getIndexPool() != -1)
		_elements[vertex.getIndexPool].deleteVertice();
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::clear()
{
	std::vector<VertexPoolElement<NBR_ATTRIBUTE>>::iterator it;

	it = _elements.begin();
	while (it != _element.end())
	{
		if ((*it).isEmpty())
		{
			_sizeBuffer -= (*it).getVertex().getSizeBuffer();
			_sizeIndices -= (*it).getVertex().getSizeIndices();
			it = _elements.erase(it);
		}
		++it;
	}
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::fullClear()
{
	_elements.clear();
	_sizeBuffer = 0;
	_sizeIndices = 0;
}

template <uint8_t NBR_ATTRIBUTE>
size_t VertexPool<NBR_ATTRIBUTE>::getNbrElement() const
{
	return (_element.size());
}

template <uint8_t NBR_ATTRIBUTE>
GLuint VertexPool<NBR_ATTRIBUTE>::getPointerAttribute(GLint index) const
{
	return (_pointerAttributes[index]);
}

#endif /*!VERTEXPOOL_HH_*/