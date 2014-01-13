#ifdef VERTEXPOOL_HH_

template <uint8_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::VertexPool(std::array<Attribute, 4> const &attributes)
: _sizeVertexBuffer(0),
_sizeIndicesBuffer(0),
_nbrVertex(0),
_nbrIndices(0),
_updateMajor(false),
_updateMinor(false),
_attributes(attributes)
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
_sizeVertexBuffer(copy._sizeVertexBuffer),
_sizeIndicesBuffer(copy._sizeIndicesBuffer),
_nbrVertex(copy._nbrVertex),
_nbrIndices(copy._nbrIndices),
_pointerAttributes(copy._pointerAttributes),
_updateBuffer(copy._updateBuffer),
_updateMajor(copy._updateMajor),
_updateMinor(copy._updateMinor),
_attributes(copy._attributes)
{
}

template <uint8_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE> &VertexPool<NBR_ATTRIBUTE>::operator=(VertexPool<NBR_ATTRIBUTE> const &vertexpool)
{
	_elements = vertexpool._elements;
	_sizeVertexBuffer = vertexpool._sizeVertexBuffer;
	_sizeIndicesBuffer = vertexpool._sizeIndicesBuffer;
	_nbrVertex = vertexpool._nbrVertex;
	_nbrIndices = vertexpool._nbrIndices;
	_pointerAttributes = vertexpool._pointerAttributes;
	_updateBuffer = vertexpool._updateBuffer;
	_updateMajor = vertexpool._updateMajor;
	_updateMinor = vertexpool._updateMinor;
	_attributes = vertexpool._attributes;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE> const &VertexPool<NBR_ATTRIBUTE>::operator[](uint32_t index)
{
	return (_element[index]);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::computeOffset()
{
	VertexPoolElement<NBR_ATTRIBUTE> element(vertex);
	uint32_t indicesOffset = 0;
	uint32_t vertexOffset = 0;
	std::array<uint32_t, NBR_ATTRIBUTE> byteOffset;

	if (element.size() > 0)
	{
		_pointerAttributes[0] = 0;
		_byteOffset[0] = 0;
		for (uint8_t index = 1; index < NBR_ATTRIBUTE; ++index)
		{
			_pointerAttribute[index] = _attributes[index - 1].getSizeType * _attributes[index - 1].getNbrComponent * _nbrVertex;
			byteOffset[index] = _pointerAttributes[index];
		}
		_elements[0].settingOffset(0, 0, byteOffset);
		for (uint32_t index = 1; index < _elements.size(); ++index)
		{
			VertexPoolElement<NBR_ATTRIBUTE> element(_elements[index - 1]);
			indicesOffset = element.getIndicesOffset() + element.getVertex().getNbrIndices();
			vertexOffset = element.getVertexOffset() + element.getVertex().getNbrVertex();
			for (uint8_t index = 0; index < NBR_ATTRIBUTE; ++index)
			{
				byteOffset[index] =
					element.getByteOffset(index) +
					(element.getVertex().getNbrVertex() *
					_attributes[index].getSizeType() *
					_attributes[index].getNbrComponent());
			}
			_elements[index].settingOffset(indicesOffset, vertexOffset, byteOffset);
		}
	}
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPool<NBR_ATTRIBUTE>::addElement(Vertice<NBR_ATTRIBUTE> const &vertices)
{
	uint32_t sizeNotUse = 0;
	std::array<uint32_t, NBR_ATTRIBUTE> byteOffset;
	uint32_t vertexOffset;
	uint32_t indicesOffset;

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
	_elements.push_back(VertexPoolElement(1, vertices));
	_sizeIndicesBuffer += vertex.getSizeIndicesBuffer();
	_sizeVertexBuffer += vertex.getSizeVertexBuffer();
	_nbrVertex += vertex.getNbrVertex();
	_nbrIndices += vertex.getNbrIndices();
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
			_sizeIndicesBuffer += (*it).getSizeIndicesBuffer();
			_sizeVertexBuffer += (*it).getSizeVertexBuffer();
			_nbrVertex += (*it).getNbrVertex();
			_nbrIndices += (*it).getNbrIndices();
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
uint32_t VertexPool<NBR_ATTRIBUTE>::getSizeIndicesBuffer() const
{
	return (_sizeIndicesBuffer);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPool<NBR_ATTRIBUTE>::getSizeVertexBuffer() const
{
	return (_sizeVertexBuffer);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPool<NBR_ATTRIBUTE>::getNbrVertex() const
{
	return (_nbrVertex);
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPool<NBR_ATTRIBUTE>::getNbrIndices() const
{
	return (_nbrIndices);
}

template <uint8_t NBR_ATTRIBUTE>
GLuint VertexPool<NBR_ATTRIBUTE>::getPointerAttribute(GLint index) const
{
	return (_pointerAttributes[index]);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::updateMinor() const
{

}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::updateMajor() const
{

}

template <uint8_t NBR_ATTRIBUTE>
bool VertexPool<NBR_ATTRIBUTE>::update()
{
	if (_updateMajor)
		updateMajor();
	else if (_updateMinor)
		updateMinor();
}

#endif /*!VERTEXPOOL_HH_*/