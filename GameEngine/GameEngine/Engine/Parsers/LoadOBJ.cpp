#include "ResourceManager/SharedMesh.hh"
#include "Parsers.hh"
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Utils/File.hpp"

bool	loadObj(std::string const &path, std::vector<Resources::Geometry> &geometry)
{
	File file(path);

	if (!file.exists())
		return false;

	// using tyny obj : https://github.com/syoyo/tinyobjloader

    std::string inputfile = file.getFullName();
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, inputfile.c_str(), file.getFolder().c_str());
	assert(err.empty());

	geometry.resize(shapes.size());

    for (size_t i = 0; i < shapes.size(); i++)
	{
		loadObjShape(shapes[i], geometry[i]);

	}
	return true;
}

void loadObjShape(tinyobj::shape_t &shape, Resources::Geometry &geometry)
{
	std::vector<glm::vec4> resPos;
	std::vector<glm::vec4> resNorm;
	std::vector<glm::vec2> resUv;
	std::vector<glm::vec4> resCol;

		for (size_t v = 0; v < shape.mesh.indices.size(); ++v)
		{
			unsigned int p = shape.mesh.indices[v] * 3;
			unsigned int p2 = shape.mesh.indices[v] * 2;

			if (shape.mesh.positions.size() > 0)
				resPos.push_back(glm::vec4(shape.mesh.positions[p],
				shape.mesh.positions[p + 1],
				shape.mesh.positions[p + 2], 1));
			if (shape.mesh.normals.size() > 0)
				resNorm.push_back(glm::vec4(shape.mesh.normals[p],
				shape.mesh.normals[p + 1],
				shape.mesh.normals[p + 2], 1));
			if (shape.mesh.texcoords.size() > 0)
				resUv.push_back(glm::vec2(shape.mesh.texcoords[p2],
				shape.mesh.texcoords[p2 + 1]));
			resCol.push_back(glm::vec4(1));
			geometry.indices.push_back(v);
		}
		geometry.colors = resCol;
		geometry.uvs = resUv;
		geometry.normals = resNorm;
		geometry.vertices = resPos;
}