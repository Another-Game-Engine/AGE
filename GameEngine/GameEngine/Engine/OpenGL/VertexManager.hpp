#ifdef VERTEXMANAGER_HH_

template <uint8_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager(std::array<Attribute, NBR_ATTRIBUTE> const &attributes)
: _indexBuffer(true),
_pool(attributes),
_attributes(attributes)
{
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_isAttributeActivate[index] = false;
}

template <uint8_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::~VertexManager()
{
	_vertexArray.bind();
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
	if (_isAttributeActivate[index] == true)
		glDisableVertexAttribArray(index);
	_vertexArray.unbind();
	_indexBuffer.unbind();
	_dataBuffer.unbind();
}

template <uint8_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager(VertexManager<NBR_ATTRIBUTE> const &copy)
: _vertexArray(copy._vertexArray),
_indexBuffer(copy._indexBuffer),
_dataBuffer(copy._dataBuffer),
_pool(copy._pool),
_isAttributeActivate(copy._isAttributeActivate),
_attributes(copy._attributes)
{
}

template <uint8_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE> &VertexManager<NBR_ATTRIBUTE>::operator=(VertexManager<NBR_ATTRIBUTE> const &vertexmanager)
{
	_vertexArray = vertexmanager._vertexArray;
	_indiceBuffer = vertexmanager._indexBuffer;
	_dataBuffer = vertexmanager._dataBuffer;
	_pool = vertexmanager._pool;
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_isAttributeActivate[index] = pool._isAttributeActivate[index];
	_attributes = vertexmanager._attribute;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
bool VertexManager<NBR_ATTRIBUTE>::init()
{
	_vertexArray.init();
	return (true);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::addVertice(Vertice<NBR_ATTRIBUTE> &vertex)
{
	std::size_t index;

	index = _pool.addElement(vertex);
	vertex._index = index;
	vertex._vertexManager = this;
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::deleteVertice(Vertice<NBR_ATTRIBUTE> &vertice)
{
	_pool.deleteElement(vertice);
	vertice._index = -1;
	vertice._vertexManager = NULL;
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::sendVertexAttribPointerOnGPU()
{
	for (GLuint index = 0; index < GLuint(NBR_ATTRIBUTE); ++index)
	{
		if (_isAttributeActivate[index] == false)
		{
			glEnableVertexAttribArray(index);
			_isAttributeActivate[index] = true;
		}
		glVertexAttribPointer
			(index,
			_attributes[index].getNbrComponent(),
			_attributes[index].getType(),
			GL_FALSE,
			0,
			reinterpret_cast<GLvoid const *>(_pool.getPointerAttribute(index)));
	}
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::sendMajorVertexDataOnGPU()
{
	_pool.computeOffset();
	_vertexArray.bind();
	_dataBuffer.bind();
	_indexBuffer.bind();
	sendVertexAttribPointerOnGPU();
	glBufferData(GL_ARRAY_BUFFER, _pool.getSizeVertexBuffer(), NULL, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _pool.getSizeIndicesBuffer(), NULL, GL_STREAM_DRAW);
	for (size_t index = 0; index < _pool.getNbrElement(); ++index)
	{
		for (uint8_t attribute = 0; attribute < NBR_ATTRIBUTE; ++attribute)
		{
			glBufferSubData
				(GL_ARRAY_BUFFER,
				_pool[index].getByteOffset(attribute),
				_pool[index].getNbrByte(attribute),
				_pool[index].getVertex().getBuffer(attribute));
		}
		glBufferSubData
			(GL_ELEMENT_ARRAY_BUFFER,
			_pool[index].getIndicesOffset(),
			_pool[index].getVertex().getSizeIndicesBuffer(),
			_pool[index].getVertex().getIndices());
	}
	_vertexArray.unbind();
	_pool.resetState();
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::sendMinorVertexDataOnGPU()
{
	std::size_t index;
	_dataBuffer.bind();
	_indexBuffer.bind();
	while (_pool.getUpdateMinor(index))
	{
		for (uint8_t attribute = 0; attribute < NBR_ATTRIBUTE; ++attribute)
		{
			glBufferSubData
				(GL_ARRAY_BUFFER,
				_pool[index].getByteOffset(attribute),
				_pool[index].getNbrByte(attribute),
				_pool[index].getVertex().getBuffer(attribute));
		}
		glBufferSubData
			(GL_ELEMENT_ARRAY_BUFFER,
			_pool[index].getIndicesOffset(),
			_pool[index].getVertex().getSizeIndicesBuffer(),
			_pool[index].getVertex().getIndices());
	}
	_pool.resetState();
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::update()
{
	if (_pool.getUpdateState() == MAJOR_UPDATE)
		sendMajorVertexDataOnGPU();
	else if (_pool.getUpdateState() == MINOR_UPDATE)
		sendMinorVertexDataOnGPU();
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::callDraw(Vertice<NBR_ATTRIBUTE> const * const drawable, GLenum mode)
{
	if (drawable->isDrawable())
	{
		update();
		_vertexArray.bind();
		if (drawable->hasIndices())
		{
			glDrawElementsBaseVertex(
				mode,
				static_cast<GLsizei>(drawable->getNbrIndices()),
				GL_UNSIGNED_INT,
				reinterpret_cast<GLvoid const *>(_pool[drawable->getIndexPool()].getIndicesOffset()),
				static_cast<GLint>(_pool[drawable->getIndexPool()].getVertexOffset())
				);
		}
		else
			glDrawArrays(mode
			, static_cast<GLint>(_pool[drawable->getIndexPool()].getVertexOffset())
			, static_cast<GLsizei>(drawable->getNbrVertex()));
	}
}

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
VertexPoolElement<NBR_ATTRIBUTE> const &VertexPool<NBR_ATTRIBUTE>::operator[](std::size_t index)
{
	return (_elements[index]);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPool<NBR_ATTRIBUTE>::computeOffset()
{
	std::size_t indicesOffset = 0;
	std::size_t vertexOffset = 0;
	std::array<std::size_t, NBR_ATTRIBUTE> byteOffset;
	std::array<std::size_t, NBR_ATTRIBUTE> nbrByte;

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
		for (std::size_t index = 1; index < _elements.size(); ++index)
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
std::size_t VertexPool<NBR_ATTRIBUTE>::addElement(Vertice<NBR_ATTRIBUTE> const &vertices)
{
	std::size_t sizeNotUse = 0;
	std::size_t vertexOffset = 0;
	std::size_t indicesOffset = 0;
	AddVerticesResult ret = FAIL;

	for (std::size_t index = 0; index < _elements.size(); ++index)
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
		auto &v = (*it).getVertex();
		if ((*it).isEmpty())
		{
			_sizeIndicesBuffer += v.getSizeIndicesBuffer();
			_sizeVertexBuffer += v.getSizeVertexBuffer();
			_nbrVertex += v.getNbrVertex();
			_nbrIndices += v.getNbrIndices();
			it = _elements.erase(it);
		}
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
std::size_t VertexPool<NBR_ATTRIBUTE>::getSizeIndicesBuffer() const
{
	return (_sizeIndicesBuffer);
}

template <uint8_t NBR_ATTRIBUTE>
std::size_t VertexPool<NBR_ATTRIBUTE>::getSizeVertexBuffer() const
{
	return (_sizeVertexBuffer);
}

template <uint8_t NBR_ATTRIBUTE>
std::size_t VertexPool<NBR_ATTRIBUTE>::getNbrVertex() const
{
	return (_nbrVertex);
}

template <uint8_t NBR_ATTRIBUTE>
std::size_t VertexPool<NBR_ATTRIBUTE>::getNbrIndices() const
{
	return (_nbrIndices);
}

template <uint8_t NBR_ATTRIBUTE>
std::size_t VertexPool<NBR_ATTRIBUTE>::getPointerAttribute(GLint index) const
{
	return (_pointerAttributes[index]);
}

template <uint8_t NBR_ATTRIBUTE>
StateVertexPool VertexPool<NBR_ATTRIBUTE>::getUpdateState() const
{
	return (_updateState);
}

template <uint8_t NBR_ATTRIBUTE>
bool VertexPool<NBR_ATTRIBUTE>::getUpdateMinor(std::size_t &index)
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

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::VertexPoolElement(std::size_t nbrEntity, Vertice<NBR_ATTRIBUTE> const &vertices)
: _nbrEntity(nbrEntity),
_vertices(vertices),
_vertexOffset(0),
_indicesOffset(0)
{
	for (uint8_t index = 0; index < NBR_ATTRIBUTE; index++)
	{
		_byteOffset[index] = 0;
		_nbrByte[index] = 0;
	}
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy)
: _nbrEntity(copy._nbrEntity),
_vertices(copy._vertices),
_vertexOffset(copy._vertexOffset),
_indicesOffset(copy._indicesOffset),
_byteOffset(copy._byteOffset),
_nbrByte(copy._nbrByte)
{
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE> &VertexPoolElement<NBR_ATTRIBUTE>::operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element)
{
	_nbrEntity = element._nbrEntity;
	_vertices = element._vertices;
	_vertexOffset = element._vertexOffset;
	_indicesOffset = element._indicesOffset;
	_byteOffset = element._byteOffset,
		_nbrByte = element._nbrByte;
	return (*this);
}

template <uint8_t NBR_ATTRIBUTE>
VertexPoolElement<NBR_ATTRIBUTE>::~VertexPoolElement()
{
}

template <uint8_t NBR_ATTRIBUTE>
AddVerticesResult VertexPoolElement<NBR_ATTRIBUTE>::addVertice(Vertice<NBR_ATTRIBUTE> const &vertices)
{
	if (_nbrEntity == 0)
	{
		_nbrEntity = 1;
		_vertices = vertices;
		return (SET);
	}
	if (_vertices == vertices)
	{
		_nbrEntity = _nbrEntity + 1;
		return (ADD);
	}
	return (FAIL);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPoolElement<NBR_ATTRIBUTE>::deleteVertice()
{
	if (_nbrEntity != 0)
		_nbrEntity = _nbrEntity - 1;
}

template <uint8_t NBR_ATTRIBUTE>
bool VertexPoolElement<NBR_ATTRIBUTE>::isEmpty() const
{
	if (_nbrEntity == 0)
		return (true);
	return (false);
}

template <uint8_t NBR_ATTRIBUTE>
std::size_t VertexPoolElement<NBR_ATTRIBUTE>::getVertexOffset() const
{
	return (_vertexOffset);
}

template <uint8_t NBR_ATTRIBUTE>
std::size_t VertexPoolElement<NBR_ATTRIBUTE>::getByteOffset(uint8_t index) const
{
	return (_byteOffset[index]);
}

template <uint8_t NBR_ATTRIBUTE>
std::size_t VertexPoolElement<NBR_ATTRIBUTE>::getNbrByte(uint8_t index) const
{
	return (_nbrByte[index]);
}

template <uint8_t NBR_ATTRIBUTE>
std::size_t VertexPoolElement<NBR_ATTRIBUTE>::getIndicesOffset() const
{
	return (_indicesOffset);
}

template <uint8_t NBR_ATTRIBUTE>
Vertice<NBR_ATTRIBUTE> const &VertexPoolElement<NBR_ATTRIBUTE>::getVertex() const
{
	return (_vertices);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexPoolElement<NBR_ATTRIBUTE>::settingOffset(std::size_t vertexOffset, std::size_t indicesOffset, std::array<std::size_t, NBR_ATTRIBUTE> const &byteOffset, std::array<std::size_t, NBR_ATTRIBUTE> const &nbrByte)
{
	_vertexOffset = vertexOffset;
	_indicesOffset = indicesOffset;
	_byteOffset = byteOffset;
	_nbrByte = nbrByte;
}

#endif /*VERTEXMANAGER_HH_*/