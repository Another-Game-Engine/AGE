#ifndef VERTEXPOOL_HH_
# define VERTEXPOOL_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <vector>
# include <utility>
# include <stdint.h>
# include "Vertice.hh"
# include "VertexPoolElement.hh"

template <uint16_t NBR_ATTRIBUTE>
class VertexPool
{
private:
	uint32_t _sizeIndices;
	uint32_t _sizeData;
	std::array<GLuint, NBR_ATTRIBUTE> _pointerAttributes;
public:
	VertexPool();
	~VertexPool();
	VertexPool(VertexPool const &copy);
	VertexPool &operator=(VertexPool const &vertexpool);
	int32_t addElement(Vertice<NBR_ATTRIBUTE> const &vertex);
	void deleteElement(Vertice<NBR_ATTRIBUTE> const &vertex);
	void fullClear();
	void clear();
	bool update();
	size_t getNbrElement() const;
	uint32_t getSizeIndices() const;
	uint32_t getSizeData() const;
	GLuint getPointerAttribute(GLint index) const;
};

# include "VertexPool.hpp"

#endif /*!VERTEXPOOL_HH_*/