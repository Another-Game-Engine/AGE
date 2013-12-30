#ifndef VERTEXMANAGER_HH_
# define VERTEXMANAGER_HH_

# include <iostream>
# include "Vertex.hh"

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

};

#endif /*!VERTEXMANAGER_HH_*/