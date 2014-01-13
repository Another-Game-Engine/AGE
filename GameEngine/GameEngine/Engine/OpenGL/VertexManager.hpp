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
	_vertexArray.unload();
	_indexBuffer.unload();
	_dataBuffer.unload();
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
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_isBindAttribtue[index] = copy._isBindAttribtue[index];
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
void VertexManager<NBR_ATTRIBUTE>::addVertice(Vertice<NBR_ATTRIBUTE> *vertex)
{
	int32_t index;

	if (vertex)
	{
		index = _pool.addElement(vertex);
		vertex->_index = index;
		vertex->_vertexManager = this;
	}
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::deleteVertice(Vertice<NBR_ATTRIBUTE> * const vertice)
{
	_pool.deleteElement(vertice);
	vertice->_index = -1;
	vertice->_vertexManager = NULL;
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::clear()
{
	_drawable.clear();
	_pool.fullClear();
}

template <uint8_t NBR_ATTRIBUTE>
inline void VertexManager<NBR_ATTRIBUTE>::update()
{
	_vertexArray.bind();
	_dataBuffer.bind();
	for (GLuint index = 0; index < GLuint(NBR_ATTRIBUTE); ++index)
	{
		if (_isBindAttribute[index] == false)
		{
			glEnableVertexAttribArray(index);
			_isBindAttribute[index] = true;
		}
		glVertexAttribPointer
			(index,
			_attributes[index].getNbrComponent(),
			_attributes[index].getType(),
			GL_FALSE,
			0,
			_pool.getPointerAttribute(index));
	}
	_indexBuffer.bind();
	_vertexArray.unbind();
}

template <uint8_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::callDraw(Vertice<NBR_ATTRIBUTE> const * const drawable, GLenum mode)
{
	if (drawable->isDrawable())
	{
		if (_pool.update())
			update();
		else
		{
			_vertexArray.bind();
			if (drawable->hasIndices())
				glDrawElementsBaseVertex(mode, drawable->getSizeIndices() / sizeof(uint32_t), GL_UNSIGNED_INT, _pool[drawable->getIndexPool()].vertexOffset(), _pool[drawable->getIndexPool()].getVertexOffset(), drawable->getNbrVertex());
			else
				glDrawArrays(mode, _pool[drawable->getIndexPool()].getVertexOffset(), drawable->getNbrVertex());
		}
	}
}

#endif /*VERTEXMANAGER_HH_*/