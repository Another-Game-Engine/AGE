#include "ResourceManager/SharedMesh.hh"

#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "Utils/File.hpp"
#include "tiny_obj_loader.h"

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
	// just on mesh for a single .obj for the moment
    for (size_t i = 0; i < shapes.size(); i++)
	{
		std::vector<glm::vec4> resPos;
		std::vector<glm::vec4> resNorm;
		std::vector<glm::vec2> resUv;
		std::vector<glm::vec4> resCol;

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
			geometry[i].indices.push_back(v);
		}
		geometry[i].colors = resCol;
		geometry[i].uvs = resUv;
		geometry[i].normals = resNorm;
		geometry[i].vertices = resPos;
	}
	return true;
}