#include "ResourceManager/SharedMesh.hh"

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "Utils/File.hpp"
#include "tiny_obj_loader.h"

bool	loadObj(std::string const &path, Resources::Geometry &geometry)
{
	File file(path);

	if (!file.exists())
		return false;

	// using tyny obj : https://github.com/syoyo/tinyobjloader

    std::string inputfile = file.getFullName();
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, inputfile.c_str(), file.getFolder().c_str());
	assert(err.empty());

	std::vector<glm::vec4> resPos;
	std::vector<glm::vec4> resNorm;
	std::vector<glm::vec2> resUv;
	std::vector<glm::vec4> resCol;

	// just on mesh for a single .obj for the moment
    for (size_t i = 0; i < 1; i++)
	{
		for (size_t v = 0; v < shapes[i].mesh.indices.size(); ++v)
		{
			unsigned int p = shapes[i].mesh.indices[v] * 3;
			unsigned int p2 = shapes[i].mesh.indices[v] * 2;

			if (shapes[i].mesh.positions.size() > 0)
				resPos.push_back(glm::vec4(shapes[i].mesh.positions[p],
				shapes[i].mesh.positions[p + 1],
				shapes[i].mesh.positions[p + 2], 1));
			if (shapes[i].mesh.normals.size() > 0)
				resNorm.push_back(glm::vec4(shapes[i].mesh.normals[p],
				shapes[i].mesh.normals[p + 1],
				shapes[i].mesh.normals[p + 2], 1));
			if (shapes[i].mesh.texcoords.size() > 0)
				resUv.push_back(glm::vec2(shapes[i].mesh.texcoords[p2],
				shapes[i].mesh.texcoords[p2 + 1]));
			resCol.push_back(glm::vec4(1));
			geometry.indices.push_back(v);
		}
		geometry.colors = resCol;
		geometry.uvs = resUv;
		geometry.normals = resNorm;
		geometry.vertices = resPos;

		//for (size_t v = 0; v < shapes[i].mesh.positions.size(); v += 3)
		//{
		//	geometry.vertices.push_back(glm::vec4(shapes[i].mesh.positions[v],
		//										  shapes[i].mesh.positions[v + 1],
		//										  shapes[i].mesh.positions[v + 2],
		//										  1));
		//	geometry.colors.push_back(glm::vec4(1));
		//}
		//for (size_t v = 0; v < shapes[i].mesh.normals.size(); v += 3)
		//	geometry.normals.push_back(glm::vec4(shapes[i].mesh.normals[v],
		//										 shapes[i].mesh.normals[v + 1],
		//										 shapes[i].mesh.normals[v + 2],
		//										 0));
		//for (size_t v = 0; v < shapes[i].mesh.texcoords.size(); v += 2)
		//	geometry.uvs.push_back(glm::vec2(shapes[i].mesh.texcoords[v],
		//										 shapes[i].mesh.texcoords[v + 1]));
		//for (size_t v = 0; v < shapes[i].mesh.indices.size(); v += 1)
		//	geometry.indices.push_back(shapes[i].mesh.indices[v]);
	}
	return true;
}

  //virtual ObjModelMedia			*load(const File &file, bool force = false)
  //{

  //  GLuint				vertexBuffer;

  //  glGenBuffers(1, &vertexBuffer);
  //  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  //  glBufferData(GL_ARRAY_BUFFER, resVertices.size() * sizeof(glm::vec3), &resVertices[0], GL_STATIC_DRAW);
  //  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //  GLuint				uvBuffer;

  //  glGenBuffers(1, &uvBuffer);
  //  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  //  glBufferData(GL_ARRAY_BUFFER, resUvs.size() * sizeof(glm::vec2), &resUvs[0], GL_STATIC_DRAW);
  //  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //  return new ObjModelMedia(vertexBuffer, uvBuffer, resVertices.size(), file.getFileName(), force);
  //}


//bool	loadObj(std::string const &path, Resources::Geometry &geometry)
//{
//  std::vector<glm::vec4> _vertices;
//  std::vector<glm::vec4> _normals;
//  std::vector<glm::vec2> _uvs;
//  std::vector<unsigned int> _vertexIndices;
//  std::vector<unsigned int> _normalIndices;
//  std::vector<unsigned int> _uvIndices;
//  bool _hasNormals;
//  bool _hasUvs;
//
//  FILE *file;
//  char type[128];
//
//  file = fopen(path.c_str(), "r");
//  if (file == NULL)
//    return false;
//  while (fscanf(file, "%s", type) != EOF)
//  {
//    if (strcmp(type, "v") == 0)
//		if (_parseVertex(file, _vertices) == false)
//			return (false);
//    else if (strcmp(type, "vt") == 0)
//		if (_parseUv(file, _hasUvs, _uvs) == false)
//			return (false);
//    else if (strcmp(type, "vn") == 0)
//		if (_parseNormal(file, _hasNormals, _normals) == false)
//			return (false);
//    else if (strcmp(type, "f") == 0)
//		if (_parseFace(file, _hasUvs, _hasNormals, _vertexIndices, _uvIndices, _normalIndices) == false)
//			return (false);
//    else // skip this line.
//    {
//      char buf[1024];
//      fgets(buf, 1024, file);
//    }
//  }
//
//  unsigned int vertexIndex;
//  unsigned int uvIndex;
//  unsigned int normalIndex;
//
//  glm::vec4 vertex;
//  glm::vec4 normal;
//  glm::vec2 uv;
//
//  geometry.normals.clear();
//  geometry.vertices.clear();
//  geometry.colors.clear();
//  geometry.uvs.clear();
//  // For each vertex of each triangle
//  for (unsigned int i = 0; i < _vertexIndices.size(); ++i)
//  {
//
//    // Get the indices of its attributes
//    vertexIndex = _vertexIndices[i];
//    normalIndex = _normalIndices[i];
//    if (_hasUvs)
//      uvIndex = _uvIndices[i];
//
//    // Get the attributes thanks to the index
//    vertex = _vertices[ vertexIndex-1 ];
//    normal = _normals[ normalIndex-1 ];
//    if (_hasUvs)
//      uv = _uvs[ uvIndex-1 ];
//
//    // Put the attributes in buffers
//    geometry.vertices.push_back(vertex);
//    geometry.uvs.push_back(_hasUvs ? uv : glm::vec2());
//    geometry.normals.push_back(normal);
//    geometry.indices.push_back(i);
//    geometry.colors.push_back(glm::vec4(1, 1, 1, 1));
//  }
//  return true;
//}