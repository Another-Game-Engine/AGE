#ifndef		SHAREDMESH_HH_
#define		SHAREDMESH_HH_

#include "AResource.hh"
#include "OpenGL/VertexBuffer.hh"
#include <Utils/SmartPointer.hh>
#include "glm/glm.hpp"

class Material;

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
	std::vector<Geometry>		_geometry;
	std::vector<OpenGLTools::VertexBuffer>	_buffer;
	std::vector<SmartPointer<Material> > _defaultMaterialsList;
public:
	SharedMesh(void);
	virtual ~SharedMesh(void);

	virtual bool	load(std::string const &path);

	void			draw() const;

	std::vector<OpenGLTools::VertexBuffer>	&getBuffer();
	std::vector<Geometry>      &getGeometry();
	inline std::vector<SmartPointer<Material> > &getDefaultMaterialsList() { return _defaultMaterialsList; }
};

}

#endif