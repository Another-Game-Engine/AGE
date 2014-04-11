#ifndef VERTICE_HH_
# define VERTICE_HH_

# include <array>
# include "Data.hh"
# include "VertexManager.hh"

template <uint8_t NBR_ATTRIBUTE> class VertexManager;

template <uint8_t NBR_ATTRIBUTE>
class Vertice
{
public:
	Vertice();
	Vertice(std::size_t nbrVertex, std::array<Data, NBR_ATTRIBUTE> attribData, Data const * const indices = NULL);
	~Vertice();
	Vertice(Vertice const &copy);
	Vertice<NBR_ATTRIBUTE> &operator=(Vertice<NBR_ATTRIBUTE> const &vertex);
	bool operator==(Vertice<NBR_ATTRIBUTE> const &vertex) const;
	std::size_t getSizeVertexBuffer() const;
	std::size_t getSizeIndicesBuffer() const;
	std::size_t getNbrIndices() const;
	std::size_t getNbrVertex() const;
	std::size_t getIndexPool() const;
	void const * const getBuffer(uint8_t index) const;
	void const * const getIndices() const;
	bool isDrawable() const;
	bool hasIndices() const;
	void draw(GLenum mode = GL_TRIANGLES) const;
	friend void VertexManager<NBR_ATTRIBUTE>::addVertice(Vertice<NBR_ATTRIBUTE> &vertice);
	friend void VertexManager<NBR_ATTRIBUTE>::deleteVertice(Vertice<NBR_ATTRIBUTE> &vertice);

private:
	std::array<Data, NBR_ATTRIBUTE> _bufferData;
	std::size_t _sizeVertexBuffer;
	std::size_t _sizeIndicesBuffer;
	std::size_t _nbrVertex;
	std::size_t _nbrIndices;
	Data *_indices;
	VertexManager<NBR_ATTRIBUTE> *_vertexManager;
	std::size_t _index;
};

# include "Vertice.hpp"
# include "VertexManager.hh"

#endif /* !VERTICE_HH_ */