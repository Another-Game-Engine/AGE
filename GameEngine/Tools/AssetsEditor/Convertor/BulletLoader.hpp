#pragma once

#include "AssimpLoader.hpp"
#include "SkeletonLoader.hpp"

#include <map>
#include <Skinning/Skeleton.hpp>
#include <Geometry/Mesh.hpp>
#include <glm/glm.hpp>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>

namespace AGE
{
	class BulletLoader
	{
	public:
		static bool save(AssetDataSet &dataSet)
		{
			if (dataSet.staticShape)
			{
				auto folderPath = std::tr2::sys::path(dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder());

				if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
				{
					std::cerr << "Bullet convertor error : creating directory" << std::endl;
					return false;
				}
				auto fileName = dataSet.skinName.empty() ? dataSet.filePath.getShortFileName() + "_static.phage" : dataSet.skinName + "_static.phage";
				auto name = dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder() + fileName;

				std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
				btDefaultSerializer	serializer;
				serializer.startSerialization();
				dataSet.staticShape->serializeSingleShape(&serializer);
				serializer.finishSerialization();
				ofs.write((const char *)(serializer.getBufferPointer()), serializer.getCurrentBufferSize());
			}
			if (dataSet.dynamicShape)
			{
				auto folderPath = std::tr2::sys::path(dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder());

				if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
				{
					std::cerr << "Bullet convertor error : creating directory" << std::endl;
					return false;
				}
				auto fileName = dataSet.skinName.empty() ? dataSet.filePath.getShortFileName() + "_dynamic.phage" : dataSet.skinName + "_dynamic.phage";
				auto name = dataSet.serializedDirectory.path().directory_string() + "\\" + dataSet.filePath.getFolder() + fileName;

				std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
				btDefaultSerializer	serializer;
				serializer.startSerialization();
				dataSet.dynamicShape->serializeSingleShape(&serializer);
				serializer.finishSerialization();
				ofs.write((const char *)(serializer.getBufferPointer()), serializer.getCurrentBufferSize());
			}
			return true;
		}

		static bool load(AssetDataSet &dataSet)
		{
			if (!dataSet.mesh)
				return false;
			auto &meshs = dataSet.mesh->subMeshs;

			//STATIC ----------------------------------
			dataSet.staticTriangleMesh = std::make_shared<btTriangleMesh>();
			std::size_t indiceNb = 0;
			std::size_t verticeNb = 0;
			for (auto &e : dataSet.mesh->subMeshs)
			{
				indiceNb += e.indices.size();
				verticeNb += e.positions.size();
			}
			dataSet.staticTriangleMesh->preallocateIndices(indiceNb);
			dataSet.staticTriangleMesh->preallocateVertices(verticeNb);
			for (std::size_t j = 0; j < meshs.size(); ++j)
			{
				auto &geo = meshs[j];
				for (std::size_t i = 0; i < geo.indices.size(); i += 3)
				{
					auto a = geo.positions[geo.indices[i]];
					auto b = geo.positions[geo.indices[i+1]];
					auto c = geo.positions[geo.indices[i+2]];
					dataSet.staticTriangleMesh->addTriangle(btVector3(a.x, a.y, a.z)
						, btVector3(b.x, b.y, b.z)
						, btVector3(c.x, c.y, c.z), false);
				}
			}
			dataSet.staticShape = std::make_shared<btBvhTriangleMeshShape>(dataSet.staticTriangleMesh.get(), true);
			dataSet.staticShape->buildOptimizedBvh();

			//DYNAMIC ----------------------------------
			{
				auto &geos = dataSet.mesh->subMeshs;
				dataSet.dynamicShape = std::make_shared<btConvexHullShape>();
				for (std::size_t i = 0; i < geos.size(); ++i)
				{
					auto &geo = geos[i];
					btScalar *t = new btScalar[geo.indices.size() * 3]();
					for (std::size_t it = 0; it < geo.indices.size(); ++it)
					{
						auto id = geo.indices[it];
						t[it * 3] = geo.positions[id].x;
						t[it * 3 + 1] = geo.positions[id].y;
						t[it * 3 + 2] = geo.positions[id].z;
					}
					btConvexHullShape *tmp = new btConvexHullShape(t, static_cast<int>(geo.indices.size()), 3 * sizeof(btScalar));
					btShapeHull *hull = new btShapeHull(tmp);
					btScalar margin = tmp->getMargin();
					hull->buildHull(margin);
					tmp->setUserPointer(hull);
					for (int it = 0; it < hull->numVertices(); ++it)
					{
						dataSet.dynamicShape->addPoint(hull->getVertexPointer()[std::size_t(it)], false);
					}
					dataSet.dynamicShape->recalcLocalAabb();
					btTransform localTrans;
					delete[] t;
					delete hull;
					delete tmp;
				}
			}
			return true;
		}
	};
}