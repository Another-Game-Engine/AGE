#ifndef VERTEX_HH_
# define VERTEX_HH_


# include <stdint.h>
# include <array>

class VertexData
{
private:
	uint32_t _id;
	uint32_t _nbrByte;
	uint32_t _nbrComponent;
	uint32_t _nbrElement;
	uint32_t _sizeBuffer;
	void *_buffer;
public:
	VertexData(uint32_t nbrByte, uint32_t nbrComponent, uint32_t nbrBuffer, void *buffer);
	VertexData(VertexData const &copy);
	~VertexData();
	VertexData &operator=(VertexData const &vertexdata);
	uint32_t getNbrComponent() const;
	uint32_t getNbrElement() const;
	uint32_t getNbrByte() const;
	uint32_t getSizeBuffer() const;
	void const * const getBuffer() const;
};

template <uint32_t NBR_ATTRIBUTE, uint32_t NBR_UNIFORM>
class Vertex
{
public:
	Vertex(std::array<VertexData, NBR_ATTRIBUTE> attributes, std::array<VertexData, NBR_UNIFORM> uniforms);
	~Vertex();
	Vertex(Vertex const &copy);
	Vertex &operator=(Vertex const &vertex);
private:
	std::array<VertexData, NBR_ATTRIBUTE> _attributes;
	std::array<VertexData, NBR_UNIFORM> _uniforms;
};

template <uint32_t NBR_ATTRIBUTE, uint32_t NBR_UNIFORM>
Vertex<NBR_ATTRIBUTE, NBR_UNIFORM>::Vertex(std::array<VertexData, NBR_ATTRIBUTE> attributes, std::array<VertexData, NBR_UNIFORM> uniforms)
: _attributes(attributes), _uniforms(_uniforms)
{
}

template <uint32_t NBR_ATTRIBUTE, uint32_t NBR_UNIFORM>
Vertex<NBR_ATTRIBUTE, NBR_UNIFORM>::Vertex(Vertex<NBR_ATTRIBUTE, NBR_UNIFORM> const &copy)
: _attributes(copy._attributes), _uniforms(copy._uniforms)
{
}

template <uint32_t NBR_ATTRIBUTE, uint32_t NBR_UNIFORM>
Vertex<NBR_ATTRIBUTE, NBR_UNIFORM>::~Vertex()
{
}

template <uint32_t NBR_ATTRIBUTE, uint32_t NBR_UNIFORM>
Vertex<NBR_ATTRIBUTE, NBR_UNIFORM> &Vertex<NBR_ATTRIBUTE, NBR_UNIFORM>::operator=(Vertex<NBR_ATTRIBUTE, NBR_UNIFORM> const &vertex)
{
	_attributes = vertex._attributes;
	_uniforms = vertex._uniforms;
	return (*this);
}
#endif /*!VERTEX_HH_*/