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
	VertexPoolElement(std::size_t nbrEntity, Vertice<NBR_ATTRIBUTE> const &vertices);
	VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy);
	~VertexPoolElement();
	VertexPoolElement &operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element);

	AddVerticesResult addVertice(Vertice<NBR_ATTRIBUTE> const &vertices);
	void deleteVertice();

	bool isEmpty() const;
	std::size_t getVertexOffset() const;
	std::size_t getIndicesOffset() const;
	std::size_t getByteOffset(uint8_t index) const;
	std::size_t getNbrByte(uint8_t index) const;
	Vertice<NBR_ATTRIBUTE> const &getVertex() const;

	inline void settingOffset(std::size_t vertexOffset, std::size_t indicesOffset, std::array<std::size_t, NBR_ATTRIBUTE> const &byteOffset, std::array<std::size_t, NBR_ATTRIBUTE> const &nbrByte);

private:
	std::size_t _nbrEntity;
	Vertice<NBR_ATTRIBUTE> _vertices;
	std::size_t _vertexOffset;
	std::size_t _indicesOffset;
	std::array<std::size_t, NBR_ATTRIBUTE> _byteOffset;
	std::array<std::size_t, NBR_ATTRIBUTE> _nbrByte;
};

#include "VertexPoolElement.hpp"

#endif /*!VERTEXPOOLELEMENT_HH_*/