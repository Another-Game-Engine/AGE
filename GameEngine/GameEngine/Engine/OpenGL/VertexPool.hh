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

enum StateVertexPool
{
	MINOR_UPDATE = 0,
	MAJOR_UPDATE,
	NO_UPDATE
};

template <uint8_t NBR_ATTRIBUTE>
class VertexPool
{
private:
	std::vector<VertexPoolElement<NBR_ATTRIBUTE>> _elements;
	std::size_t _sizeVertexBuffer;
	std::size_t _sizeIndicesBuffer;
	std::size_t _nbrVertex;
	std::size_t _nbrIndices;
	std::array<std::size_t, NBR_ATTRIBUTE> _pointerAttributes;
	std::queue<std::size_t> _updateBuffer;
	StateVertexPool _updateState;
	std::array<Attribute, NBR_ATTRIBUTE> _attributes;
public:
	VertexPool(std::array<Attribute, NBR_ATTRIBUTE> const &attributes);
	~VertexPool();
	VertexPool(VertexPool<NBR_ATTRIBUTE> const &copy);
	VertexPool &operator=(VertexPool const &vertexpool);
	
	std::size_t addElement(Vertice<NBR_ATTRIBUTE> const &vertices);
	void deleteElement(Vertice<NBR_ATTRIBUTE> const &vertices);
	void fullClear();
	void clear();
	inline void computeOffset();

	VertexPoolElement<NBR_ATTRIBUTE> const &operator[](std::size_t index);
	size_t getNbrElement() const;
	std::size_t getSizeIndicesBuffer() const;
	std::size_t getSizeVertexBuffer() const;
	std::size_t getNbrVertex() const;
	std::size_t getNbrIndices() const;
	std::size_t getPointerAttribute(GLint index) const;
	StateVertexPool getUpdateState() const;
	bool getUpdateMinor(std::size_t &index);
	void resetState();
};

# include "VertexPool.hpp"

#endif /*!VERTEXPOOL_HH_*/