#ifndef VERTEXMANAGER_HH_
# define VERTEXMANAGER_HH_

# include <iostream>
# include <unordered_map>
# include "Vertex.hh"
# include "VertexArray.hh"
# include "VertexBuffer.hh"

template <uint16_t NBR_ATTRIBUTE, uint16_t NBR_UNIFORM>
class VertexManager
{
public:
	VertexManager();
	~VertexManager();
	VertexManager(VertexManager const &copy);
	VertexManager &operator=(VertexManager const &vertexmanager);
	void addVertex(Vertex<NBR_ATTRIBUTE, NBR_UNIFORM> const &vertex, std::string const &name);
	void deleteVertex(std::string const &name);
	void clear();
	void draw();
private:
	OpenGLTools::VertexArray _vertexArray;
	OpenGLTools::VertexBuffer _indexBuffer;
	OpenGLTools::VertexBuffer _dataBuffer;
	std::unordered_map<std::string, Vertex<NBR_ATTRIBUTE, NBR_UNIFORM> const *> _entity;
	std::vector< Vertex<NBR_ATTRIBUTE, NBR_UNIFORM> > _pool;
};

#endif /*!VERTEXMANAGER_HH_*/