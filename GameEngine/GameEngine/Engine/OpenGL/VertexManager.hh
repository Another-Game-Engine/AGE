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
# include "VertexPool.hh"

template <uint16_t NBR_ATTRIBUTE>
class VertexManager
{
public:
	class Vertex<NBR_ATTRIBUTE>;

	struct PoolElement
	{
		uint32_t nbrEntity;
		Vertex<NBR_ATTRIBUTE> vertex;
		PoolElement(uint32_t nbrEntity, Vertex<NBR_ATTRIBUTE> vertex);
		PoolElement(PoolElement const &copy);
		~PoolElement();
		PoolElement &operator=(PoolElement const &element);
		bool operator==(PoolElement const &element);
		bool operator!=(PoolElement const &element);
		bool operator==(Vertex<NBR_ATTRIBUTE> const &element);
		bool operator!=(Vertex<NBR_ATTRIBUTE> const &element);
		Data const &operator[](size_t index) const;
	};
public:
	VertexManager();
	~VertexManager();
	VertexManager(VertexManager const &copy);
	VertexManager &operator=(VertexManager const &vertexmanager);
	void addVertex(std::string const &name, Vertex<NBR_ATTRIBUTE> *vertex);
	void deleteVertex(std::string const &name);
	void clear();
	inline void sendToGPU(GLenum mode = GL_STREAM);
private:
	OpenGLTools::VertexArray _vertexArray;
	OpenGLTools::VertexBuffer _indexBuffer;
	OpenGLTools::VertexBuffer _dataBuffer;
	bool _updatePool;
	VertexPool<NBR_ATTRIBUTE> _pool;
	std::unordered_map<std::string, Vertex<NBR_ATTRIBUTE> *> _drawable;
	bool _isBindAttribtue[NBR_ATTRIBUTE];
private:
	inline void addVertexInPool(Vertex<NBR_ATTRIBUTE> * vertex);
	inline void addVertexInDrawable(std::string const &name, Vertex<NBR_ATTRIBUTE> *vertex);
	inline void deleteVertexInPool(Vertex<NBR_ATTRIBUTE> * const vertex);
	Vertex<NBR_ATTRIBUTE> * const deleteVertexInDrawable(std::string const &name);
	inline void sendToGPUVertexAttribPointer();
};

# include "PoolElement.hpp"
# include "VertexManager.hpp"

#endif /*!VERTEXMANAGER_HH_*/