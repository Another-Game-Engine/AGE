#ifndef VERTEXPOOLELEMENT_HH_
# define VERTEXPOOLELEMENT_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <stdint.h>
# include "Vertice.hh"

enum AddVerticeResult
{
	FAIL = 0,
	SET,
	ADD
};

template <uint8_t NBR_ATTRIBUTE>
class VertexPoolElement
{
public:
	VertexPoolElement(uint32_t nbrEntity, Vertice<NBR_ATTRIBUTE> vertices);
	VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy);
	~VertexPoolElement();
	VertexPoolElement &operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element);

	bool addVertice(Vertice<NBR_ATTRIBUTE> const &vertices);
	void deleteVertice();

	bool isEmpty() const;
	uint32_t getVertexOffset() const;
	uint32_t getIndicesOffset() const;
	uint32_t getByteOffset(uint8_t index) const;
	void setByteOffset(uint8_t index, uint32_t offset);
	Vertice<NBR_ATTRIBUTE> const &getVertex() const;

	inline void settingOffset(uint32_t vertexOffset, uint32_t indicesOffset, std::array<uint32_t, NBR_ATTRIBUTE> byteOffset);

private:
	uint32_t _nbrEntity;
	Vertice<NBR_ATTRIBUTE> _vertices;
	uint32_t _vertexOffset;
	uint32_t _indicesOffset;
	std::array<uint32_t, NBR_ATTRIBUTE> _byteOffset;
};

#include "VertexPoolElement.hpp"

#endif /*!VERTEXPOOLELEMENT_HH_*/