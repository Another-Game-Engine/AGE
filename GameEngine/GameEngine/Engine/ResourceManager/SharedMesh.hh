#ifndef		SHAREDMESH_HH_
#define		SHAREDMESH_HH_

#include <iostream>
#include <array>
#include <Utils/SmartPointer.hh>
#include "AResource.hh"
#include "OpenGL/VertexArray.hh"
#include "OpenGL/VertexBuffer.hh"
#include "OpenGL/Vertice.hh"
#include "OpenGL/Attribute.hh"
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
	std::vector<Vertice<4>> _objs;
	std::vector<SmartPointer<Material> > _defaultMaterialsList;
	VertexManager<4> *_manager;

public:
	SharedMesh(void);
	virtual ~SharedMesh(void);

	virtual bool	load(std::string const &path);

	void			draw() const;

	std::vector<Geometry>      &getGeometry();
	std::vector<Vertice<4>> const &getDrawable() const;
	inline std::vector<SmartPointer<Material> > &getDefaultMaterialsList() { return _defaultMaterialsList; }
};

}

#endif