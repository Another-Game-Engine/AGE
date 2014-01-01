#ifndef VERTEXMANAGER_HH_
# define VERTEXMANAGER_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <unordered_map>
# include <vector>
# include <utility>
# include "Vertex.hh"
# include "VertexArray.hh"
# include "VertexBuffer.hh"

template <uint16_t NBR_ATTRIBUTE>
class VertexManager
{
public:
	struct Pool
	{
		std::vector<PoolElement> poolElements;
		uint32_t sizeIndices;
		uint32_t sizeData;
		Pool();
		~Pool();
		Pool(Pool const &copy);
		Pool &operator=(Pool const &pool);
		Pool &operator+=(Vertex<NBR_ATTRIBUTE> const &vertex);
		bool operator==(Vertex<NBR_ATTRIBUTE> const &vertex) const;
		bool operator!=(Vertex<NBR_ATTRIBUTE> const &vertex) const;
	};
	struct PoolElement
	{
		uint32_t nbrEntity;
		Vertex<NBR_ATTRIBUTE> vertex;
		PoolElement(uint32_t nbrEntity, Vertex<NBR_ATTRIBUTE> vertex);
		PoolElement(PoolElement const &copy);
		PoolElement &operator=(PoolElement const &element);
		~PoolElement();
		bool operator==(PoolElement const &element);
		bool operator!=(PoolElement const &element);
		bool operator==(Vertex<NBR_ATTRIBUTE> const &element);
		bool operator!=(Vertex<NBR_ATTRIBUTE> const &element);
	};
public:
	VertexManager();
	~VertexManager();
	VertexManager(VertexManager const &copy);
	VertexManager &operator=(VertexManager const &vertexmanager);
	void addVertex(Vertex<NBR_ATTRIBUTE> const &vertex, std::string const &name);
	void deleteVertex(std::string const &name);
	void sendToGraphicProcessUnit(GLenum mode = GL_STREAM) const;
	void clear();
	void draw(GLenum mode = GL_TRIANGLES);
private:
	OpenGLTools::VertexArray _vertexArray;
	OpenGLTools::VertexBuffer _indexBuffer;
	OpenGLTools::VertexBuffer _dataBuffer;
	Pool _pool;
};

# include "Pool.hpp"
# include "VertexManager.hpp"


#endif /*!VERTEXMANAGER_HH_*/