#ifndef VERTEX_HH_
# define VERTEX_HH_

# include "Data.hh"

template <uint32_t NBR_ATTRIBUTE>
class Vertex
{
public:
	class VertexManager<NBR_ATTRIBUTE>;

public:
	Vertex(std::array<Data, NBR_ATTRIBUTE> attributes, Data const * const indices = NULL);
	~Vertex();
	Vertex(Vertex const &copy);
	Vertex &operator=(Vertex const &vertex);
	std::array<Data, NBR_ATTRIBUTE> const &getAttributes() const;
	Data const * const getIndices() const;
	bool operator==(Vertex<NBR_ATTRIBUTE> const &vertex) const;
	bool operator!=(Vertex<NBR_ATTRIBUTE> const &vertex) const;
	uint32_t getSizeAttributes() const;
	uint32_t getSizeIndices() const;
	friend void VertexManager<NBR_ATTRIBUTE>::addVertex(std::string const &name, Vertex<NBR_ATTRIBUTE> *vertex);
	bool isDrawable() const;
	void draw() const;
private:
	std::array<Data, NBR_ATTRIBUTE> _attributes;
	Data *_indices;
	uint32_t _sizeAttributes;
	uint32_t _sizeIndices;
	VertexManager<NBR_ATTRIBUTE> *_vertexManager;
	int32_t _indexPool;
};

# include "Vertex.hpp"

#endif /* !VERTEX_HH_ */