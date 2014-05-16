#ifndef VERTICE_HH_
# define VERTICE_HH_

#define TEST_ARCHI 1

#if TEST_ARCHI

# define WARNING_MESSAGE_BUFFER(index) \
	std::cerr << "Warning: the index to get the buffer into vertices [" << index << "] is out of range." << std::endl;\

# define WARNING_MESSAGE_SIZEBUFFER(index) \
	std::cerr << "Warning: the index to get the size buffer into vertices [" << index << "] is out of range." << std::endl;\

#include <stdint.h>

namespace gl
{
	//!\file Vertices.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class Vertices
	//!\brief contain vertices information
	class Vertices
	{
	public:
		Vertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers);
		~Vertices();
		Vertices(Vertices const &copy);
		Vertices &operator=(Vertices const &v);

		void const *getBuffer(uint8_t index) const;
		size_t getSizeBuffer(uint8_t index) const;
		uint8_t getNbrBuffers() const;
		size_t getNbrVertices() const;
	private:
		void **_buffers;
		size_t *_sizeBuffers;
		size_t _nbrVertices;
		uint8_t _nbrBuffers;
	};
}

#else
# include "Data.hh"

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
#endif

#endif /* !VERTICE_HH_ */