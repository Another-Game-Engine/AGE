#ifndef VERTEX_HH_
# define VERTEX_HH_

# include <stdint.h>

struct VertexData
{
	uint32_t nbrComponent;
	uint32_t nbrElement;
	uint32_t nbrByte;
	uint32_t sizeBuffer;
	void *buffer;
};

template <unsigned int NBR_ATTRIBUTE, unsigned int NBR_UNIFORM>
class Vertex
{
public:
	Vertex();
	Vertex();
	Vertex(Vertex const &copy);
	Vertex &operator=(Vertex const &vertex);
	void addAttribute(VertexData const &attribute);
	void deleteAttribute(uint16_t index);
private:
	VertexData _attribute[NBR_ATTRIBUTE];
	VertexData _uniform[NBR_UNIFORM];
};

#endif /*!VERTEX_HH_*/