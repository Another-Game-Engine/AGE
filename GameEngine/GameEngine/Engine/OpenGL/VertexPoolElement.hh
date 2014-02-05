#ifndef VERTEXPOOLELEMENT_HH_
# define VERTEXPOOLELEMENT_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <stdint.h>
# include "Vertice.hh"

enum AddVerticesResult
{
	FAIL = 0,
	SET,
	ADD
};

template <uint8_t NBR_ATTRIBUTE>
class VertexPoolElement
{
public:
	VertexPoolElement(uint32_t nbrEntity, Vertice<NBR_ATTRIBUTE> const &vertices);
	VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy);
	~VertexPoolElement();
	VertexPoolElement &operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element);

	AddVerticesResult addVertice(Vertice<NBR_ATTRIBUTE> const &vertices);
	void deleteVertice();

	bool isEmpty() const;
	uint32_t getVertexOffset() const;
	uint32_t getIndicesOffset() const;
	uint32_t getByteOffset(uint8_t index) const;
	uint32_t getNbrByte(uint8_t index) const;
	Vertice<NBR_ATTRIBUTE> const &getVertex() const;

	inline void settingOffset(uint32_t vertexOffset, uint32_t indicesOffset, std::array<uint32_t, NBR_ATTRIBUTE> const &byteOffset, std::array<uint32_t, NBR_ATTRIBUTE> const &nbrByte);

private:
	uint32_t _nbrEntity;
	Vertice<NBR_ATTRIBUTE> _vertices;
	uint32_t _vertexOffset;
	uint32_t _indicesOffset;
	std::array<uint32_t, NBR_ATTRIBUTE> _byteOffset;
	std::array<uint32_t, NBR_ATTRIBUTE> _nbrByte;
};

#include "VertexPoolElement.hpp"

#endif /*!VERTEXPOOLELEMENT_HH_*/