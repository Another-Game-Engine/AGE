#ifndef		SHAREDMESH_HH_
#define		SHAREDMESH_HH_

#include "AResource.hh"
#include "OpenGL/VertexArray.hh"
#include "OpenGL/VertexBuffer.hh"
#include "OpenGL/Vertice.hh"

#include "glm/glm.hpp"

namespace	Resources
{

struct Geometry
{
  std::vector<glm::vec4>		vertices;	// vertices positions
  std::vector<glm::vec4>		normals;	// normals
  std::vector<glm::vec4>		colors;		// vertices colors
  std::vector<glm::vec2>		uvs;		// texture coordinates
  std::vector<unsigned int>		indices;	// indices
};

class SharedMesh : public AResource
{
private:
	Geometry					_geometry;
	OpenGLTools::VertexArray	_buffer;
	Vertice<4>					*_obj;
	VertexManager<4>			*_manager;
public:
	SharedMesh(void);
	virtual ~SharedMesh(void);

	virtual bool	load(std::string const &path);

	void			draw() const;

	OpenGLTools::VertexArray	&getBuffer();
	const Geometry      &getGeometry() const;
};

}

#endif