#include "ResourceManager/SharedMesh.hh"
#include "Parsers.hh"
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "Utils/File.hpp"
#include <core/Engine.hh>
#include <core/Renderer.hh>

bool	loadObj(std::string const &path, Resources::SharedMesh &mesh)
{
	File file(path);

	if (!file.exists())
		return false;

	// using tyny obj : https://github.com/syoyo/tinyobjloader

    std::string inputfile = file.getFullName();
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, inputfile.c_str(), file.getFolder().c_str());
	assert(err.empty());
	auto &geometry = mesh.getGeometry();
	geometry.resize(shapes.size());
	mesh.getDefaultMaterialsList().resize(shapes.size());

	float min = 0.0f;
	float max = 0.0f;
    for (size_t i = 0; i < shapes.size(); i++)
	{
		for (auto &e : shapes[i].mesh.positions)
		{
			if (e < min)
				min = e;
			else if (e > max)
				max = e;
		}
	}

	float minCof = std::abs(min) * 2.0f;
	float maxCof = max * 2.0f;
    for (size_t i = 0; i < shapes.size(); i++)
	{
		for (auto &e : shapes[i].mesh.positions)
		{
			if (e < 0)
				e /= minCof;
			else if (e > 0)
				e /= maxCof;
		}
	}


    for (size_t i = 0; i < shapes.size(); i++)
	{
		loadObjShape(shapes[i], geometry[i]);
		loadObjMaterials(shapes[i], mesh, file, i);
	}
	return true;
}

void    loadObjMaterials(tinyobj::shape_t &shape, Resources::SharedMesh &mesh, const File &objPath, unsigned int index)
{
	auto &m = mesh.getDefaultMaterialsList();
	auto &manager = mesh.getEngine()->getInstance<Renderer>().getMaterialManager();
	auto name = "material:" + File(shape.material.name).getFileName();
	m[index] = manager.getMaterial(name);
	if (m[index] != nullptr)
		return;
	auto material = manager.createMaterial(name);
	material->loadMtl(shape.material, objPath);
	m[index] = material;
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