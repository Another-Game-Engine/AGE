#include "ResourceManager/SharedMesh.hh"

#include <string>
#include <vector>

#include "glm/glm.hpp"

bool	loadObj(std::string const &path, Geometry &geometry)
{
  std::vector<glm::vec4> _vertices;
  std::vector<glm::vec4> _normals;
  std::vector<glm::vec2> _uvs;
  std::vector<unsigned int> _vertexIndices;
  std::vector<unsigned int> _normalIndices;
  std::vector<unsigned int> _uvIndices;
  bool _hasNormals;
  bool _hasUvs;

  FILE *file;
  char type[128];

  file = fopen(path.c_str(), "r");
  if (file == NULL)
    return false;
  while (fscanf(file, "%s", type) != EOF)
  {
    if (strcmp(type, "v") == 0)
		if (_parseVertex(file, _vertices) == false)
			return (false);
    else if (strcmp(type, "vt") == 0)
		if (_parseUv(file, _hasUvs, _uvs) == false)
			return (false);
    else if (strcmp(type, "vn") == 0)
		if (_parseNormal(file, _hasNormals, _normals) == false)
			return (false);
    else if (strcmp(type, "f") == 0)
		if (_parseFace(file, _hasUvs, _hasNormals, _vertexIndices, _uvIndices, _normalIndices) == false)
			return (false);
    else // skip this line.
    {
      char buf[1024];
      fgets(buf, 1024, file);
    }
  }

  unsigned int vertexIndex;
  unsigned int uvIndex;
  unsigned int normalIndex;

  glm::vec4 vertex;
  glm::vec4 normal;
  glm::vec2 uv;

  geometry.normals.clear();
  geometry.vertices.clear();
  geometry.colors.clear();
  geometry.uvs.clear();
  // For each vertex of each triangle
  for (unsigned int i = 0; i < _vertexIndices.size(); ++i)
  {

    // Get the indices of its attributes
    vertexIndex = _vertexIndices[i];
    normalIndex = _normalIndices[i];
    if (_hasUvs)
      uvIndex = _uvIndices[i];

    // Get the attributes thanks to the index
    vertex = _vertices[ vertexIndex-1 ];
    normal = _normals[ normalIndex-1 ];
    if (_hasUvs)
      uv = _uvs[ uvIndex-1 ];

    // Put the attributes in buffers
    geometry.vertices.push_back(vertex);
    geometry.uvs.push_back(_hasUvs ? uv : glm::vec2());
    geometry.normals.push_back(normal);
    geometry.indices.push_back(i);
    geometry.colors.push_back(glm::vec4(1, 1, 1, 1));
  }
  return true;
}

/**
 * Extract the vertex data from the current line.
 */
bool _parseVertex(FILE *file, std::vector<glm::vec4> &_vertices)
{
  glm::vec4 vertex;
  int ret = fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
  vertex.w = 1;
	if (ret != 3)
		return (false);
  _vertices.push_back(vertex);
  return (true);
}

/**
 * Extract the normal data from the current line.
 */
bool _parseNormal(FILE *file, bool &_hasNormals, std::vector<glm::vec4> &_normals)
{
  _hasNormals = true;
  glm::vec4 normal;
  int ret = fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
  normal.w = 0;
	if (ret != 3)
		return (false);
  _normals.push_back(normal);
  return (true);
}

/**
 * Extract the uv coords from the current line.
 */
bool _parseUv(FILE *file, bool &_hasUvs, std::vector<glm::vec2> &_uvs)
{
  _hasUvs = true;
  glm::vec2 uv;
  int ret = fscanf(file, "%f %f\n", &uv.x, &uv.y);
  if (ret != 2)
	  return (false);
  _uvs.push_back(uv);
  return (true);
}

/**
 * Extract the face from the current line.
 */
bool _parseFace(FILE *file, bool _hasUvs, bool _hasNormals,
				std::vector<unsigned int> &_vertexIndices,
				std::vector<unsigned int> &_uvIndices,
				std::vector<unsigned int> &_normalIndices)
{
  std::string vertex1, vertex2, vertex3;
  unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

  if (_hasUvs && _hasNormals)
  {
    int matches = fscanf(
      file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
      &vertexIndex[0], &uvIndex[0], &normalIndex[0],
      &vertexIndex[1], &uvIndex[1], &normalIndex[1],
      &vertexIndex[2], &uvIndex[2], &normalIndex[2]
    );
	if (matches != 9)
		return (false);
    _vertexIndices.push_back(vertexIndex[0]);
    _vertexIndices.push_back(vertexIndex[1]);
    _vertexIndices.push_back(vertexIndex[2]);
    _uvIndices.push_back(uvIndex[0]);
    _uvIndices.push_back(uvIndex[1]);
    _uvIndices.push_back(uvIndex[2]);
    _normalIndices.push_back(normalIndex[0]);
    _normalIndices.push_back(normalIndex[1]);
    _normalIndices.push_back(normalIndex[2]);
  }
  else if (!_hasUvs && _hasNormals)
  {
    int matches = fscanf(
      file, "%d//%d %d//%d %d//%d\n",
      &vertexIndex[0], &normalIndex[0],
      &vertexIndex[1], &normalIndex[1],
      &vertexIndex[2], &normalIndex[2]
    );
	if (matches != 6)
		return (false);
    _vertexIndices.push_back(vertexIndex[0]);
    _vertexIndices.push_back(vertexIndex[1]);
    _vertexIndices.push_back(vertexIndex[2]);
    _normalIndices.push_back(normalIndex[0]);
    _normalIndices.push_back(normalIndex[1]);
    _normalIndices.push_back(normalIndex[2]);
  }
  return (true);
}