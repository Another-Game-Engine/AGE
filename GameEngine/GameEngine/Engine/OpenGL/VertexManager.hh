#ifndef VERTEXMANAGER_HH_
# define VERTEXMANAGER_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <unordered_map>
# include <vector>
# include <utility>
# include "Vertex.hh"
# include "VertexArray.hh"
# include "VertexBuffer.hh"

template <uint16_t NBR_ATTRIBUTE>
class VertexManager
{
public:
	VertexManager();
	~VertexManager();
	VertexManager(VertexManager const &copy);
	VertexManager &operator=(VertexManager const &vertexmanager);
	void addVertex(Vertex<NBR_ATTRIBUTE> const &vertex, std::string const &name);
	void deleteVertex(std::string const &name);
	void sendToGraphicProcessUnit(GLenum mode = GL_STREAM) const;
	void clear();
	void draw(GLenum mode = GL_TRIANGLES);
private:
	bool _updateOnGPU;
	OpenGLTools::VertexArray _vertexArray;
	OpenGLTools::VertexBuffer _indexBuffer;
	OpenGLTools::VertexBuffer _dataBuffer;
	std::unordered_map<std::string, uint32_t > _entity;
	std::vector<t_pair<uint32_t, Vertex<NBR_ATTRIBUTE>>> _poolGPU;
	uint32_t _sizeAttributesPoolGPU;
	uint32_t _sizeIndicesPoolGPU;
};

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager()
: _updateOnGPU(true),
	_sizeAttributePoolGPU(0),
	_sizeIndicesPoolGPU(0)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager()
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE>::VertexManager(VertexManager<NBR_ATTRIBUTE> const &copy)
: _updateOnGPU(copy._updateOnGPU),
	_vertexArray(copy._vertexArray),
	_indexBuffer(copy._indexBuffer),
	_dataBuffer(copy._dataBuffer),
	_entity(copy._entity),
	_poolGPU(copy._poolGPU),
	_sizeAttributePoolGPU(copy._sizeAttributePoolGPU),
	_sizeIndicesPoolGPU(copy._sizeIndicesPoolGPU)
{
}

template <uint16_t NBR_ATTRIBUTE>
VertexManager<NBR_ATTRIBUTE> &VertexManager<NBR_ATTRIBUTE>::operator=(VertexManager<NBR_ATTRIBUTE> const &vertexmanager)
{
	_updateOnGPU = vertexmanager._updateOnGPU;
	_vertexArray = vertexmanager._vertexArray;
	_indiceBuffer = vertexmanager._indexBuffer;
	_dataBuffer = vertexmanager._dataBuffer;
	_entity = vertexmanager._entity;
	_poolGPU = vertexmanager._poolGPU;
	_sizeAttributePoolGPU = vertexmanager._sizeAttributePoolGPU;
	_sizeIndicesPoolGPU = vertexmanager._sizeIndicesPoolGPU;
	return (*this);
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::addVertex(Vertex<NBR_ATTRIBUTE> const &vertex, std::string const &name)
{
	bool alreadyPresent = false;

	for (uint32_t index = 0; index < _poolGPU.size(); ++index)
	{
		if (_poolGPU[index].second == vertex)
		{
			_poolGPU[index].first = _poolGPU[index].first + 1;
			_entity[name] = index;
			alreadyPresent = true;
		}
	}
	if (alreadyPresent == false)
	{
		_updateOnGPU = true;
		_poolGPU.push_back(std::pair<uint32_t, Vertex<NBR_ATTRIBUTE>>(1, vertex));
		_entity[name] = _poolGPU.size() - 1;
		_sizeAttributesPoolGPU += _poolGPU.back().getSizeAttributes();
		_sizeIndicesPoolGPU += _poolGPU.back().getSizeIndices();
	}
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::deleteVertex(std::string const &name)
{
	if ((auto &it = _entity.find(name)) != _entity.end())
	{
		_poolGPU[_entity[name]].first -= 1;
		_entity.erase(it);
	}
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::clear()
{
	for (uint32_t index = 0; index < _poolGPU.size(); ++index)
		_poolGPU[index].first = 0;
	_entity.clear();
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::draw(GLenum mode)
{
	if (_updateOnGPU)
		sendToGraphicProcessUnit();
	_vertexArray.bind();
	//draw
	_vertexArray.unbind();
}

template <uint16_t NBR_ATTRIBUTE>
void VertexManager<NBR_ATTRIBUTE>::sendToGraphicProcessUnit(GLenum mode) const
{
	_vertexArray->bind();
	_vertexArray->unbind();
}

#endif /*!VERTEXMANAGER_HH_*/