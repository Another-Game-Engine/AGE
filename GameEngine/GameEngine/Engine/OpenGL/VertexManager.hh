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
	class Vertex<NBR_ATTRIBUTE>;

	class Pool
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
		bool _updatePool;
		bool _startPool;
	public:
		Pool();
		~Pool();
		Pool(Pool const &copy);
		Pool &operator=(Pool const &pool);
		int32_t &operator+=(Vertex<NBR_ATTRIBUTE> const &vertex);
		int32_t operator==(Vertex<NBR_ATTRIBUTE> const &vertex) const;
		int32_t operator!=(Vertex<NBR_ATTRIBUTE> const &vertex) const;
		void clear();
		size_t size() const;
		GLint sizeAttribute(GLint index) const;
		GLenum typeAttribute(GLint index) const;
		GLenum normalizedAttribute(GLint index) const;
		GLsizei strideAttribute(GLint index) const;
		GLvoid const *pointerAttribute(GLint index) const;
	private:
		bool checkDataVertex(Vertex<NBR_ATTRIBUTE> const &vertex) const;
	};
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
	Pool _pool;
	std::unordered_map<std::string, Vertex<NBR_ATTRIBUTE> *> _drawable;
	bool _isBindAttribtue[NBR_ATTRIBUTE];
private:
	inline void addVertexInPool(Vertex<NBR_ATTRIBUTE> * vertex);
	inline void addVertexInDrawable(std::string const &name, Vertex<NBR_ATTRIBUTE> *vertex);
	inline void deleteVertexInPool(Vertex<NBR_ATTRIBUTE> * const vertex);
	Vertex<NBR_ATTRIBUTE> * const deleteVertexInDrawable(std::string const &name);
	inline void sendToGPUVertexAttribPointer();
};

# include "Pool.hpp"
# include "PoolElement.hpp"
# include "VertexManager.hpp"

#endif /*!VERTEXMANAGER_HH_*/