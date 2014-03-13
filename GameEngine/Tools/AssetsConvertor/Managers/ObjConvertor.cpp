#include <Managers/ObjConvertor.hh>
#include <Managers/AssetsConvertorManager.hh>
#include <tiny_obj_loader.h>
#include <cassert>
#include <MediaFiles/ObjFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <MediaFiles/CollisionShapeStaticFile.hpp>
#include <MediaFiles/CollisionShapeDynamicFile.hpp>
#include <MediaFiles/CollisionBoxFile.hpp>
#include <MediaFiles/CollisionSphereFile.hpp>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <tuple>
#include <glm/gtx/matrix_cross_product.hpp>

ObjConvertor::ObjConvertor(AssetsConvertorManager *manager)
: AConvertor(manager, std::set<std::string>({ "obj" }))
{}

ObjConvertor::~ObjConvertor()
{}

std::string ObjConvertor::setName(const File &file) const
{
	std::string obj = file.getShortFileName();
	obj = "obj__" + obj;
	return obj;
}


std::shared_ptr<AMediaFile> ObjConvertor::convert(const File &file)
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
    for (std::size_t i = 0; i < shapes.size(); i++)
	{
		mesh->geometries[i].name = shapes[i].name;
		for (std::size_t v = 0; v < shapes[i].mesh.indices.size(); ++v)
		{
			std::size_t p = static_cast<std::size_t>(shapes[i].mesh.indices[v]) * std::size_t(3);
			std::size_t p2 = static_cast<std::size_t>(shapes[i].mesh.indices[v]) * std::size_t(2);
			auto &m = shapes[i].mesh;
			if (m.positions.size() > 0)
				mesh->geometries[i].vertices.push_back(glm::vec4(m.positions[p],
				m.positions[p + std::size_t(1)],
				m.positions[p + std::size_t(2)], 1));
			if (m.normals.size() > 0)
				mesh->geometries[i].normals.push_back(glm::vec4(m.normals[p],
				m.normals[p + std::size_t(1)],
				m.normals[p + std::size_t(2)], 1));
			if (m.texcoords.size() > 0)
				mesh->geometries[i].uvs.push_back(glm::vec2(m.texcoords[p2],
				m.texcoords[p2 + std::size_t(1)]));
			mesh->geometries[i].colors.push_back(glm::vec4(1));
			mesh->geometries[i].indices.push_back(static_cast<unsigned int>(v));
		}
	}
	auto mtl = file.getFullName().substr(0, file.getFullName().find_last_of("."));
	mtl += ".mtl";
	mesh->material = std::static_pointer_cast<MaterialFile>(_manager->load(mtl));
	if (mesh->material != nullptr)
		mesh->incrementChilds();

	/////
	///
	///
	/// CONVERT FOR BULLET

	// STATIC CONCAVE SHAPE
	{
		std::shared_ptr<btTriangleMesh> trimesh{new btTriangleMesh()};
		auto &geos = mesh->geometries;

		for (std::size_t j = 0; j < geos.size(); ++j)
		{
			auto &geo = geos[j];
			for (std::size_t i = 0; i < geo.vertices.size(); i += 3)
			{
				trimesh->addTriangle(btVector3(geo.vertices[i].x, geo.vertices[i].y, geo.vertices[i].z)
					, btVector3(geo.vertices[i + std::size_t(1)].x, geo.vertices[i + std::size_t(1)].y, geo.vertices[i + std::size_t(1)].z)
					, btVector3(geo.vertices[i + std::size_t(2)].x, geo.vertices[i + std::size_t(2)].y, geo.vertices[i + std::size_t(2)].z), true);
			}
		}
		std::shared_ptr<btBvhTriangleMeshShape> bvh{ new btBvhTriangleMeshShape(trimesh.get(), true) };
		bvh->buildOptimizedBvh();		
		std::shared_ptr<CollisionShapeStaticFile> staticShape{ new CollisionShapeStaticFile() };
		staticShape->shape = bvh;
		staticShape->trimesh = trimesh;
		staticShape->name = "collision_shape_static_" + file.getShortFileName();
		staticShape->path = File("./Assets/Serialized/" + staticShape->name + ".bullet");
		_manager->add(staticShape);
	}

	// DYNAMIC SHAPE
	{
		auto &geos = mesh->geometries;
		std::shared_ptr<btConvexHullShape> s{ new btConvexHullShape() };
		for (std::size_t i = 0; i < geos.size(); ++i)
		{
			auto &geo = geos[i];
			btScalar *t = new btScalar[geo.vertices.size() * 3]();
			for (std::size_t it = 0; it < geo.vertices.size(); ++it)
			{
				t[it * 3] = geo.vertices[it].x;
				t[it * 3 + 1] = geo.vertices[it].y;
				t[it * 3 + 2] = geo.vertices[it].z;
			}
			btConvexHullShape *tmp = new btConvexHullShape(t, static_cast<int>(geo.vertices.size()), 3 * sizeof(btScalar));
			btShapeHull *hull = new btShapeHull(tmp);
			btScalar margin = tmp->getMargin();
			hull->buildHull(margin);
			tmp->setUserPointer(hull);
			for (int it = 0; it < hull->numVertices(); ++it)
			{
				s->addPoint(hull->getVertexPointer()[std::size_t(it)], false);
			}
			s->recalcLocalAabb();
			btTransform localTrans;
			delete[] t;
			delete hull;
			delete tmp;
		}
		std::shared_ptr<CollisionShapeDynamicFile> dynamicShape{ new CollisionShapeDynamicFile() };
		dynamicShape->shape = s;
		dynamicShape->name = "collision_shape_dynamic_" + file.getShortFileName();
		dynamicShape->path = File("./Assets/Serialized/" + dynamicShape->name + ".bullet");
		_manager->add(dynamicShape);
	}

	std::vector<std::tuple<float, float, float, float, float, float>> minAndMax;
	minAndMax.resize(mesh->geometries.size());
	std::tuple<float, float, float, float, float, float> totalMinMax{0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};

	for (std::size_t i = 0; i < mesh->geometries.size(); ++i)
	{
		std::get<0>(minAndMax[i]) = mesh->geometries[i].vertices[0].x;
		std::get<1>(minAndMax[i]) = mesh->geometries[i].vertices[0].x;
		std::get<2>(minAndMax[i]) = mesh->geometries[i].vertices[0].y;
		std::get<3>(minAndMax[i]) = mesh->geometries[i].vertices[0].y;
		std::get<4>(minAndMax[i]) = mesh->geometries[i].vertices[0].z;
		std::get<5>(minAndMax[i]) = mesh->geometries[i].vertices[0].z;

		std::get<0>(totalMinMax) = mesh->geometries[i].vertices[0].x;
		std::get<1>(totalMinMax) = mesh->geometries[i].vertices[0].x;
		std::get<2>(totalMinMax) = mesh->geometries[i].vertices[0].y;
		std::get<3>(totalMinMax) = mesh->geometries[i].vertices[0].y;
		std::get<4>(totalMinMax) = mesh->geometries[i].vertices[0].z;
		std::get<5>(totalMinMax) = mesh->geometries[i].vertices[0].z;


		for (std::size_t it = 1; it < mesh->geometries[i].vertices.size(); ++it)
		{
			std::get<0>(minAndMax[i]) = std::get<0>(minAndMax[i]) < mesh->geometries[i].vertices[it].x ? std::get<0>(minAndMax[i]) : mesh->geometries[i].vertices[it].x;
			std::get<1>(minAndMax[i]) = std::get<1>(minAndMax[i]) > mesh->geometries[i].vertices[it].x ? std::get<1>(minAndMax[i]) : mesh->geometries[i].vertices[it].x;
			std::get<2>(minAndMax[i]) = std::get<2>(minAndMax[i]) < mesh->geometries[i].vertices[it].y ? std::get<2>(minAndMax[i]) : mesh->geometries[i].vertices[it].y;
			std::get<3>(minAndMax[i]) = std::get<3>(minAndMax[i]) > mesh->geometries[i].vertices[it].y ? std::get<3>(minAndMax[i]) : mesh->geometries[i].vertices[it].y;
			std::get<4>(minAndMax[i]) = std::get<4>(minAndMax[i]) < mesh->geometries[i].vertices[it].z ? std::get<4>(minAndMax[i]) : mesh->geometries[i].vertices[it].z;
			std::get<5>(minAndMax[i]) = std::get<5>(minAndMax[i]) > mesh->geometries[i].vertices[it].z ? std::get<5>(minAndMax[i]) : mesh->geometries[i].vertices[it].z;
			std::get<0>(totalMinMax) = std::get<0>(totalMinMax) < mesh->geometries[i].vertices[it].x ? std::get<0>(totalMinMax) : mesh->geometries[i].vertices[it].x; //-V573
			std::get<1>(totalMinMax) = std::get<1>(totalMinMax) > mesh->geometries[i].vertices[it].x ? std::get<1>(totalMinMax) : mesh->geometries[i].vertices[it].x; //-V573
			std::get<2>(totalMinMax) = std::get<2>(totalMinMax) < mesh->geometries[i].vertices[it].y ? std::get<2>(totalMinMax) : mesh->geometries[i].vertices[it].y; //-V573
			std::get<3>(totalMinMax) = std::get<3>(totalMinMax) > mesh->geometries[i].vertices[it].y ? std::get<3>(totalMinMax) : mesh->geometries[i].vertices[it].y; //-V573
			std::get<4>(totalMinMax) = std::get<4>(totalMinMax) < mesh->geometries[i].vertices[it].z ? std::get<4>(totalMinMax) : mesh->geometries[i].vertices[it].z; //-V573
			std::get<5>(totalMinMax) = std::get<5>(totalMinMax) > mesh->geometries[i].vertices[it].z ? std::get<5>(totalMinMax) : mesh->geometries[i].vertices[it].z; //-V573
		}
	}

	// BOX
	{
		std::shared_ptr<btBoxShape> box{new btBoxShape(btVector3(
			(std::get<1>(totalMinMax) - std::get<0>(totalMinMax)) * 0.5f,
			(std::get<3>(totalMinMax) - std::get<2>(totalMinMax)) * 0.5f,
			(std::get<5>(totalMinMax) - std::get<4>(totalMinMax)) * 0.5f
			))};
		std::shared_ptr<CollisionBoxFile> shape{ new CollisionBoxFile() };
		shape->shape = box;
		shape->name = "collision_box_" + file.getShortFileName();
		shape->path = File("./Assets/Serialized/" + shape->name + ".bullet");
		_manager->add(shape);
	}

	// SPHERE
	{
		float m = std::get<1>(totalMinMax) - std::get<0>(totalMinMax);
		m = m < std::get<3>(totalMinMax) - std::get<2>(totalMinMax) ? std::get<3>(totalMinMax) - std::get<2>(totalMinMax) : m;
		m = m < std::get<5>(totalMinMax) - std::get<4>(totalMinMax) ? std::get<5>(totalMinMax) - std::get<4>(totalMinMax) : m;
		m *= 0.5f;
		std::shared_ptr<btSphereShape> sphere{new btSphereShape(m)};
		std::shared_ptr<CollisionSphereFile> shape{ new CollisionSphereFile() };
		shape->shape = sphere;
		shape->name = "collision_sphere_" + file.getShortFileName();
		shape->path = File("./Assets/Serialized/" + shape->name + ".bullet");
		_manager->add(shape);
	}


	return mesh;
}