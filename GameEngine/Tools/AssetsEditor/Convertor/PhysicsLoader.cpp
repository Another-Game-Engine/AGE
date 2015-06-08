#include <map>
#include "PhysicsLoader.hpp"
#include "AssimpLoader.hpp"
#include "SkeletonLoader.hpp"
#include <Skinning/Skeleton.hpp>
#include <AssetManagement/Data/MeshData.hh>
#include <glm/glm.hpp>
#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <BulletCollision/CollisionShapes/btTriangleMesh.h>
#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include "ConvertorStatusManager.hpp"
#include <PhysX/PhysXSDK/Include/PxPhysicsAPI.h>
#include <PhysX/PhysXSDK/Include/extensions/PxExtensionsAPI.h>

namespace AGE
{
	bool PhysicsLoader::save(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->dataSet->loadPhysic)
			return true;
		if (cookingTask->staticShape)
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : saving static shape for " + cookingTask->dataSet->filePath.getShortFileName());
			auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Physics convertor error : creating directory" << std::endl;
				return false;
			}
			auto fileName = cookingTask->dataSet->filePath.getShortFileName() + "_static.phage";
			auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder() + fileName;

			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			btDefaultSerializer	serializer;
			serializer.startSerialization();
			cookingTask->staticShape->serializeSingleShape(&serializer);
			serializer.finishSerialization();
			ofs.write((const char *) (serializer.getBufferPointer()), serializer.getCurrentBufferSize());
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		}
		if (cookingTask->dynamicShape)
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : saving dynamic shape for " + cookingTask->dataSet->filePath.getShortFileName());
			auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Physics convertor error : creating directory" << std::endl;
				return false;
			}
			auto fileName = cookingTask->dataSet->filePath.getShortFileName() + "_dynamic.phage";
			auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder() + fileName;

			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			btDefaultSerializer	serializer;
			serializer.startSerialization();
			cookingTask->dynamicShape->serializeSingleShape(&serializer);
			serializer.finishSerialization();
			ofs.write((const char *) (serializer.getBufferPointer()), serializer.getCurrentBufferSize());
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		}
		return true;
	}

	bool PhysicsLoader::load(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->dataSet->loadPhysic)
			return true;
		auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : loading " + cookingTask->dataSet->filePath.getShortFileName());

		auto &meshs = cookingTask->mesh->subMeshs;

		//STATIC ----------------------------------
		if (cookingTask->dataSet->staticConcave)
		{
			cookingTask->staticTriangleMesh = std::make_shared<btTriangleMesh>();
			std::size_t indiceNb = 0;
			std::size_t verticeNb = 0;
			for (auto &e : cookingTask->mesh->subMeshs)
			{
				indiceNb += e.indices.size();
				verticeNb += e.positions.size();
			}
			cookingTask->staticTriangleMesh->preallocateIndices(static_cast<int>(indiceNb));
			cookingTask->staticTriangleMesh->preallocateVertices(static_cast<int>(verticeNb));
			for (std::size_t j = 0; j < meshs.size(); ++j)
			{
				auto &geo = meshs[j];
				for (std::size_t i = 0; i < geo.indices.size(); i += 3)
				{
					auto a = geo.positions[geo.indices[i]];
					auto b = geo.positions[geo.indices[i + 1]];
					auto c = geo.positions[geo.indices[i + 2]];
					cookingTask->staticTriangleMesh->addTriangle(btVector3(a.x, a.y, a.z)
																 , btVector3(b.x, b.y, b.z)
																 , btVector3(c.x, c.y, c.z), false);
				}
			}
			cookingTask->staticShape = std::make_shared<btBvhTriangleMeshShape>(cookingTask->staticTriangleMesh.get(), true);
			cookingTask->staticShape->buildOptimizedBvh();
		}
		//DYNAMIC ----------------------------------
		//if (cookingTask->dataSet->dynamicConcave)
		//{
		//	auto &geos = cookingTask->mesh->subMeshs;
		//	cookingTask->dynamicShape = std::make_shared<btConvexHullShape>();
		//	for (std::size_t i = 0; i < geos.size(); ++i)
		//	{
		//		auto &geo = geos[i];
		//		btScalar *t = new btScalar[geo.indices.size() * 3]();
		//		for (std::size_t it = 0; it < geo.indices.size(); ++it)
		//		{
		//			auto id = geo.indices[it];
		//			t[it * 3] = geo.positions[id].x;
		//			t[it * 3 + 1] = geo.positions[id].y;
		//			t[it * 3 + 2] = geo.positions[id].z;
		//		}
		//		btConvexHullShape *tmp = new btConvexHullShape(t, static_cast<int>(geo.indices.size()), 3 * sizeof(btScalar));
		//		btShapeHull *hull = new btShapeHull(tmp);
		//		btScalar margin = tmp->getMargin();
		//		hull->buildHull(margin);
		//		tmp->setUserPointer(hull);
		//		for (int it = 0; it < hull->numVertices(); ++it)
		//		{
		//			cookingTask->dynamicShape->addPoint(hull->getVertexPointer()[std::size_t(it)], false);
		//		}
		//		cookingTask->dynamicShape->recalcLocalAabb();
		//		btTransform localTrans;
		//		delete[] t;
		//		delete hull;
		//		delete tmp;
		//	}
		//}
		Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		return true;
	}
}