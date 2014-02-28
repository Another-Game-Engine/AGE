#ifdef VERTEXPOOL_HH_

template <uint8_t NBR_ATTRIBUTE>
VertexPool<NBR_ATTRIBUTE>::VertexPool(std::array<Attribute, NBR_ATTRIBUTE> const &attributes)
: _sizeVertexBuffer(0),
_sizeIndicesBuffer(0),
_nbrVertex(0),
_nbrIndices(0),
_updateState(NO_UPDATE),
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
_updateState(copy._updateState),
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
	_updateState = vertexpool._updateState;
	_attributes = vertexpool._attributes;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE> const &VertexPool<NBR_ATTRIBUTE>::operator[](uint32_t index)
{
	return (_elements[index]);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::computeOffset()
{
	uint32_t indicesOffset = 0;
	uint32_t vertexOffset = 0;
	std::array<uint32_t, NBR_ATTRIBUTE> byteOffset;
	std::array<uint32_t, NBR_ATTRIBUTE> nbrByte;

	if (_elements.size() > 0)
	{
		_pointerAttributes[0] = 0;
		byteOffset[0] = 0;
		nbrByte[0] = _elements[0].getVertex().getNbrVertex() * _attributes[0].getSizeType() * _attributes[0].getNbrComponent();
		for (uint8_t index = 1; index < NBR_ATTRIBUTE; ++index)
		{
			nbrByte[index] = _elements[0].getVertex().getNbrVertex() * _attributes[index].getSizeType() * _attributes[index].getNbrComponent();
			_pointerAttributes[index] = _pointerAttributes[index - 1] + _attributes[index - 1].getSizeType() * _attributes[index - 1].getNbrComponent() * _nbrVertex;
			byteOffset[index] = _pointerAttributes[index];
		}
		_elements[0].settingOffset(0, 0, byteOffset, nbrByte);
		for (uint32_t index = 1; index < _elements.size(); ++index)
		{
			VertexPoolElement<NBR_ATTRIBUTE> element(_elements[index - 1]);
			indicesOffset = element.getIndicesOffset() + element.getVertex().getSizeIndicesBuffer();
			vertexOffset = element.getVertexOffset() + element.getVertex().getNbrVertex();
			for (uint8_t attribute = 0; attribute < NBR_ATTRIBUTE; ++attribute)
			{
				nbrByte[attribute] = _elements[index].getVertex().getNbrVertex() * _attributes[attribute].getSizeType() * _attributes[attribute].getNbrComponent();
				byteOffset[attribute] = element.getByteOffset(attribute) + (element.getVertex().getNbrVertex() * _attributes[attribute].getSizeType() * _attributes[attribute].getNbrComponent());
			}
			_elements[index].settingOffset(vertexOffset, indicesOffset, byteOffset, nbrByte);
		}
	}
}

template <uint8_t NBR_ATTRIBUTE>
uint32_t VertexPool<NBR_ATTRIBUTE>::addElement(Vertice<NBR_ATTRIBUTE> const &vertices)
{
	uint32_t sizeNotUse = 0;
	uint32_t vertexOffset = 0;
	uint32_t indicesOffset = 0;
	AddVerticesResult ret = FAIL;

	for (size_t index = 0; index < _elements.size(); ++index)
	{
		if ((ret = _elements[index].addVertice(vertices)) != FAIL)
		{
			if (ret == SET)
			{
				_updateState = MINOR_UPDATE;
				_updateBuffer.push(index);
			}
			return (index);
		}
		if (_elements[index].isEmpty())
			sizeNotUse += _elements[index].getVertex().getSizeVertexBuffer();
	}
	if (_sizeVertexBuffer > 0 && (float(sizeNotUse) / float(_sizeVertexBuffer)) > 0.75)
		clear();
	_elements.push_back(VertexPoolElement<NBR_ATTRIBUTE>(1, vertices));
	_sizeIndicesBuffer += vertices.getSizeIndicesBuffer();
	_sizeVertexBuffer += vertices.getSizeVertexBuffer();
	_nbrVertex += vertices.getNbrVertex();
	_nbrIndices += vertices.getNbrIndices();
	_updateState = MAJOR_UPDATE;
	return (_elements.size() - 1);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::deleteElement(Vertice<NBR_ATTRIBUTE> const &vertex)
{
	if (vertex.getIndexPool() != -1)
		_elements[vertex.getIndexPool()].deleteVertice();
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::clear()
{
	std::vector<VertexPoolElement<NBR_ATTRIBUTE>>::iterator it;

	it = _elements.begin();
	while (it != _elements.end())
	{
		if ((*it).isEmpty())
		{
			_sizeIndicesBuffer += (*it).getVertex().getSizeIndicesBuffer();
			_sizeVertexBuffer += (*it).getVertex().getSizeVertexBuffer();
			_nbrVertex += (*it).getVertex().getNbrVertex();
			_nbrIndices += (*it).getVertex().getNbrIndices();
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
	return (_elements.size());
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
StateVertexPool VertexPool<NBR_ATTRIBUTE>::getUpdateState() const
{
	return (_updateState);
}

template <uint8_t NBR_ATTRIBUTE>
bool VertexPool<NBR_ATTRIBUTE>::getUpdateMinor(uint32_t &index)
{
	if (_updateBuffer.empty())
		return (false);
	index = _updateBuffer.front();
	_updateBuffer.pop();
	return (true);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::resetState()
{
	_updateState = NO_UPDATE;
}

#endif /*!VERTEXPOOL_HH_*/