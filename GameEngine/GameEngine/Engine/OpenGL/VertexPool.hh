#ifndef VERTEXPOOL_HH_
# define VERTEXPOOL_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <vector>
# include <utility>
# include <stdint.h>
# include <queue>
# include "Vertice.hh"
# include "VertexPoolElement.hh"

template <uint8_t NBR_ATTRIBUTE>
class VertexPool
{
private:
	std::vector<VertexPoolElement<NBR_ATTRIBUTE>> _elements;
	uint32_t _sizeIndices;
	uint32_t _sizeBuffer;
	std::array<GLuint, NBR_ATTRIBUTE> _pointerAttributes;
	std::queue<uint32_t> _updateBuffer;
	bool _updateMajor;
	bool _updateMinor;
public:
	VertexPool();
	~VertexPool();
	VertexPool(VertexPool<NBR_ATTRIBUTE> const &copy);
	VertexPool &operator=(VertexPool const &vertexpool);
	VertexPoolElement<NBR_ATTRIBUTE> const &operator[](uint32_t index);
	uint32_t addElement(Vertice<NBR_ATTRIBUTE> const &vertices);
	void deleteElement(Vertice<NBR_ATTRIBUTE> const &vertices);
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