#ifdef VERTEXMANAGER_HH_

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager()
: _indexBuffer(true),
_updatePool(true)
{
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_isBindAttribtue[index] = false;
}


template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::~VertexManager()
{
	_vertexArray.bind();
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
	if (_isBindAttribtue[index] == true)
		glDisableVertexAttribArray(index);
	_vertexArray.unload();
	_indexBuffer.unload();
	_dataBuffer.unload();
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager(VertexManager<NBR_ATTRIBUTE> const &copy)
: _vertexArray(copy._vertexArray),
_indexBuffer(copy._indexBuffer),
_dataBuffer(copy._dataBuffer),
_updatePool(copy._updatePool)
{
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_isBindAttribtue[index] = copy._isBindAttribtue[index];
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE> &VertexManager<NBR_ATTRIBUTE>::operator=(VertexManager<NBR_ATTRIBUTE> const &vertexmanager)
{
	_vertexArray = vertexmanager._vertexArray;
	_indiceBuffer = vertexmanager._indexBuffer;
	_dataBuffer = vertexmanager._dataBuffer;
	_updatePool = vertexmanager._updatePool;
	for (uint16_t index = 0; index < NBR_ATTRIBUTE; ++index)
		_isBindAttribtue[index] = pool._isBindAttribtue[index];
	return (*this);
}

template <uint16_t NBR_ATTRIBTUE>
void VertexManager<NBR_ATTRIBUTE>::addVertexInPool(Vertex<NBR_ATTRIBUTE> *vertex)
{
	int32_t index;

	if ((index = (_pool != vertex)) == -1)
	{
		if ((index = int32_t((_pool += vertex)) != -1)
		{
			_updatePool = true;
			vertex->_indexPoolVertexManager = index;
		}
	}
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBTUE>::addVertexInDrawable(std::string const &name, Vertex<NBR_ATTRIBUTE> *vertex)
{
	if (vertex->_indexPoolVertexManager == -1)
	{
		vertex->_vertexManager = this;
		_drawable[name] = vertex;
	}
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::addVertex(std::string const &name, Vertex<NBR_ATTRIBUTE> *vertex)
{
	if (vertex)
	{
		addVertexInDrawable(name, vertex);
		addVertexInPool(vertex);
	}
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::deleteVertexInPool(Vertex<NBR_ATTRIBUTE> * const vertex)
{
	vertex->_vertexManager = NULL;
	vertex->_indexPoolVertexManager = -1;
}

template <uint16_t NBR_ATTRIBUTE>
Vertex<NBR_ATTRIBUTE> * const VertexManager<NBR_ATTRIBUTE>::deleteVertexInDrawable(std::string const &name)
{
	std::unordered_map<std::string, Vertex<NBR_ATTRIBUTE> *>::iterator it = _drawable.find(name);
	if (it == std::unordered_map<std::string, Vertex<NBR_ATTRIBUTE> *>::end)
		return (NULL);
	else
	{
		Vertex<NBR_ATTRIBUTE> *vertex = *it;
		_drawable.erase(it);
		return (vertex);
	}
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::deleteVertex(std::string const &name)
{
	deleteVertexInDrawable(deleteVertexInPool(vertex));
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::clear()
{
	_drawable.clear();
	_pool.clear();
}

template <uint16_t NBR_ATTRIBUTE>
inline void VertexManager<NBR_ATTRIBUTE>::sendToGPUVertexAttribPointer()
{
	for (GLuint index = 0; index < GLuint(NBR_ATTRIBUTE); ++index)
	{
		if (_isBindAttribtue[index] == false)
		{
			glEnableVertexAttribArray(index);
			_isBindAttribtue[index] = true;
		}
		glVertexAttribPointer(
			index,
			_pool.sizeAttribute(index),
			_pool.typeAttribute(index), 
			_pool.normalizedAttibute(index),
			_pool.strideAttribute(index),
			_pool.pointerAttribute(index));
	}
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::sendToGPU(GLenum mode)
{
	if (_updatePool)
	{
		sendToGPUVertexAttribPointer();

	}
}

#endif /*VERTEXMANAGER_HH_*/