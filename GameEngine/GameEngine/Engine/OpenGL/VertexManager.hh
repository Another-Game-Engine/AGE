#ifndef VERTEXMANAGER_HH_
# define VERTEXMANAGER_HH_

# include "Utils/OpenGL.hh"
# include <iostream>
# include <unordered_map>
# include <utility>
# include <array>
# include "Vertice.hh"
# include "Attribute.hh"
# include "VertexArray.hh"
# include "VertexBuffer.hh"
# include "VertexPool.hh"

#include <Utils/Dependency.hpp>

template <uint8_t NBR_ATTRIBUTE>
class VertexManager : public Dependency
{
public:
	VertexManager(std::array<Attribute, NBR_ATTRIBUTE> const &attributes);
	~VertexManager();
	VertexManager(VertexManager const &copy);
	VertexManager &operator=(VertexManager const &vertexmanager);
	bool init();
	void addVertice(Vertice<NBR_ATTRIBUTE> &vertice);
	void deleteVertice(Vertice<NBR_ATTRIBUTE> &vertice);
	void update();
	void clear();
	void callDraw(Vertice<NBR_ATTRIBUTE> const * const drawable, GLenum mode);

private:
	OpenGLTools::VertexArray _vertexArray;
	OpenGLTools::VertexBuffer _indexBuffer;
	OpenGLTools::VertexBuffer _dataBuffer;
	VertexPool<NBR_ATTRIBUTE> _pool;
	std::array<bool, NBR_ATTRIBUTE> _isAttributeActivate;
	std::array<Attribute, NBR_ATTRIBUTE> _attributes;
private:
	inline void sendVertexAttribPointerOnGPU();
	inline void sendMajorVertexDataOnGPU();
	inline void sendMinorVertexDataOnGPU();
};

# include "VertexManager.hpp"

#endif /*!VERTEXMANAGER_HH_*/