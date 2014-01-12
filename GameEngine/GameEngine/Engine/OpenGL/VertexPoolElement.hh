#ifndef VERTEXPOOLELEMENT_HH_
# define VERTEXPOOLELEMENT_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <stdint.h>
# include "Vertice.hh"

template <uint8_t NBR_ATTRIBUTE>
class VertexPoolElement
{
public:
	VertexPoolElement(uint32_t nbrEntity, Vertice<NBR_ATTRIBUTE> vertex);
	VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy);
	~VertexPoolElement();
	VertexPoolElement &operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element);

	bool addVertice(Vertice<NBR_ATTRIBUTE> const vertice);
	void deleteVertice();

	uint32_t getVertexOffset() const;
	uint32_t getByteOffset() const;

private:
	uint32_t _nbrEntity;
	Vertice<NBR_ATTRIBUTE> _vertex;
	uint32_t _vertexOffset;
	uint32_t _byteOffset;
};

#include "VertexPoolElement.hpp"

#endif /*!VERTEXPOOLELEMENT_HH_*/