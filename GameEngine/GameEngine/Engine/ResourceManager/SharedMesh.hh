#ifndef		SHAREDMESH_HH_
#define		SHAREDMESH_HH_

#include "AResource.hh"
#include "OpenGL/VertexBuffer.hh"

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
	OpenGLTools::VertexBuffer	_buffer;

public:
	SharedMesh(void);
	virtual ~SharedMesh(void);

	virtual bool	load(std::string const &path);

	void			draw() const;

	OpenGLTools::VertexBuffer	&getBuffer();
	const Geometry      &getGeometry() const;

};

}

#endif