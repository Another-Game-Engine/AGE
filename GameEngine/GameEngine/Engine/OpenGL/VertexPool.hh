#ifndef VERTEXPOOL_HH_
# define VERTEXPOOL_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <vector>
# include <utility>
# include "Vertex.hh"
# include "VertexPoolElement.hh"

template <uint16_t NBR_ATTRIBUTE>
class VertexPool
{
private:
	std::vector<PoolElement> _poolElements;
	uint32_t _sizeIndices;
	uint32_t _sizeData;
	GLint _sizeAttibute[NBR_ATTRIBUTE];
	GLenum _typeAttribute[NBR_ATTRIBUTE];
	GLenum _normalizedAttribute[NBR_ATTRIBUTE];
	GLsizei _strideAttribute[NBR_ATTRIBUTE];
	GLuint _pointerAttribute[NBR_ATTRIBUTE];
public:
	VertexPool();
	~VertexPool();
	VertexPool(VertexPool const &copy);
	VertexPool &operator=(VertexPool const &vertexpool);
	int32_t &operator+=(Vertex<NBR_ATTRIBUTE> const &vertex);
	void operator-=(uint32_t index);
	int32_t operator==(Vertex<NBR_ATTRIBUTE> const &vertex) const;
	int32_t operator!=(Vertex<NBR_ATTRIBUTE> const &vertex) const;
	void fullClear();
	void clear();
	size_t size() const;
	GLint sizeAttribute(GLint index) const;
	GLenum typeAttribute(GLint index) const;
	GLenum normalizedAttribute(GLint index) const;
	GLsizei strideAttribute(GLint index) const;
	GLvoid const *pointerAttribute(GLint index) const;
private:
	bool checkDataVertex(Vertex<NBR_ATTRIBUTE> const &vertex) const;
	inline void setDataAttributeAtStart();
};

# include "VertexPool.hpp"

#endif /*!VERTEXPOOL_HH_*/