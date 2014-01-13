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
	uint32_t _sizeVertexBuffer;
	uint32_t _sizeIndicesBuffer;
	uint32_t _nbrVertex;
	uint32_t _nbrIndices;
	std::array<GLuint, NBR_ATTRIBUTE> _pointerAttributes;
	std::queue<uint32_t> _updateBuffer;
	bool _updateMajor;
	bool _updateMinor;
	std::array<Attribute, 4> _attributes;
	inline void updateMajor() const;
	inline void updateMinor() const;
public:
	VertexPool(std::array<Attribute, 4> const &attributes);
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
	uint32_t getSizeIndicesBuffer() const;
	uint32_t getSizeVertexBuffer() const;
	uint32_t getNbrVertex() const;
	uint32_t getNbrIndices() const;
	GLuint getPointerAttribute(GLint index) const;
	inline void computeOffset();
};

# include "VertexPool.hpp"

#endif /*!VERTEXPOOL_HH_*/