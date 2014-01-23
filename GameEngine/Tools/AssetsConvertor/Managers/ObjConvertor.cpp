#include <Managers/ObjConvertor.hh>
#include <Managers/AssetsConvertorManager.hh>
#include <tiny_obj_loader.h>
#include <cassert>
#include <MediaFiles/ObjFile.hpp>

ObjConvertor::ObjConvertor(AssetsConvertorManager *manager)
: AConvertor(manager, std::set<std::string>({ "obj" }))
{}

ObjConvertor::~ObjConvertor()
{}

std::shared_ptr<AMediaFile> ObjConvertor::convert(const File file)
{
	if (!file.exists())
		return std::shared_ptr<AMediaFile>(nullptr);

    std::string inputfile = file.getFullName();
    std::vector<tinyobj::shape_t> shapes;
    std::string err = tinyobj::LoadObj(shapes, inputfile.c_str(), file.getFolder().c_str());
	assert(err.empty() && "Tiny Obj error loading file.");

	std::shared_ptr<ObjFile> mesh{ new ObjFile };
	mesh->geometries.resize(shapes.size());

	///////////
	// Normalizing meshs
	//

	float min = -0.5f;
	float max = 0.5f;
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

	float minCof = min != -0.5f ? std::abs(min) * 2.0f : 1.0f;
	float maxCof = min != 0.5f ? max * 2.0f : 1.0f;
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

	/////////////
	// Converting
	//
    for (size_t i = 0; i < shapes.size(); i++)
	{
		mesh->geometries[i].name = shapes[i].name;
		for (size_t v = 0; v < shapes[i].mesh.indices.size(); ++v)
		{
			unsigned int p = shapes[i].mesh.indices[v] * 3;
			unsigned int p2 = shapes[i].mesh.indices[v] * 2;

			if (shapes[i].mesh.positions.size() > 0)
				mesh->geometries[i].vertices.push_back(glm::vec4(shapes[i].mesh.positions[p],
				shapes[i].mesh.positions[p + 1],
				shapes[i].mesh.positions[p + 2], 1));
			if (shapes[i].mesh.normals.size() > 0)
				mesh->geometries[i].normals.push_back(glm::vec4(shapes[i].mesh.normals[p],
				shapes[i].mesh.normals[p + 1],
				shapes[i].mesh.normals[p + 2], 1));
			if (shapes[i].mesh.texcoords.size() > 0)
				mesh->geometries[i].uvs.push_back(glm::vec2(shapes[i].mesh.texcoords[p2],
				shapes[i].mesh.texcoords[p2 + 1]));
			mesh->geometries[i].colors.push_back(glm::vec4(1));
			mesh->geometries[i].indices.push_back(v);
		}
	}
	return mesh;
}

//void    loadObjMaterials(tinyobj::shape_t &shape, Resources::SharedMesh &mesh, const File &objPath, unsigned int index)
//{
//	auto &m = mesh.getMaterial();
//	auto &manager = mesh.getEngine()->getInstance<Renderer>().getMaterialManager();
//	auto name = "material:" + File(shape.material.name).getFileName();
//	m[index] = manager.getMaterial(name);
//	if (m[index] != nullptr)
//		return;
//	auto material = manager.createMaterial(name);
//	material->loadMtl(shape.material, objPath, mesh.getEngine());
//	m[index] = material;
//}