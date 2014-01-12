#ifndef VERTEXPOOLELEMENT_HH_
# define VERTEXPOOLELEMENT_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <stdint.h>
# include "Vertice.hh"
# include "VertexPoolElement.hh"

template <uint16_t NBR_ATTRIBUTE>
struct VertexPoolElement
{
	uint32_t nbrEntity;
	Vertex<NBR_ATTRIBUTE> vertex;
	VertexPoolElement(uint32_t nbrEntity, Vertex<NBR_ATTRIBUTE> vertex);
	VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy);
	~VertexPoolElement();
	VertexPoolElement &operator=(VertexPoolElement const &element);
	bool operator==(VertexPoolElement<NBR_ATTRIBUTE> const &element);
	bool operator!=(VertexPoolElement<NBR_ATTRIBUTE> const &element);
	bool operator==(Vertex<NBR_ATTRIBUTE> const &element);
	bool operator!=(Vertex<NBR_ATTRIBUTE> const &element);
	Data const &operator[](size_t index) const;
	bool operator!() const;
};

#include "VertexPoolElement.hpp"

#endif /*!VERTEXPOOLELEMENT_HH_*/