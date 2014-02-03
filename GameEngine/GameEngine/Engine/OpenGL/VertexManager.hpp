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
	_indexBuffer.init();
	_dataBuffer.init();
	return (true);
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::addVertice(Vertice<NBR_ATTRIBUTE> &vertex)
{
	int32_t index;

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
void VertexManager<NBR_ATTRIBUTE>::clear()
{
	_drawable.clear();
	_pool.fullClear();
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
			_pool[index].getIndicesOffset() * sizeof(unsigned int),
			_pool[index].getVertex().getNbrIndices() * sizeof(unsigned int),
			_pool[index].getVertex().getIndices());
	}
	_vertexArray.unbind();
	_pool.resetState();
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::sendMinorVertexDataOnGPU()
{
	uint32_t index;
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
			_pool[index].getIndicesOffset() * sizeof(unsigned int),
			_pool[index].getVertex().getNbrIndices() * sizeof(unsigned int),
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
		glDrawArrays(mode, _pool[drawable->getIndexPool()].getVertexOffset(), drawable->getNbrVertex());
		if (drawable->hasIndices())
			glDrawElementsBaseVertex(mode, drawable->getNbrIndices(), GL_UNSIGNED_INT, reinterpret_cast<GLvoid const *>(_pool[drawable->getIndexPool()].getIndicesOffset()), _pool[drawable->getIndexPool()].getVertexOffset());
		else
			glDrawArrays(mode, _pool[drawable->getIndexPool()].getVertexOffset(), drawable->getNbrVertex());
	}
}

#endif /*VERTEXMANAGER_HH_*/