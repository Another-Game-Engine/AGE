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
	static bool SaveBulletConvex(std::shared_ptr<CookingTask> cookingTask)
	{
		if (cookingTask->convexShape)
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : saving convex shape for " + cookingTask->dataSet->filePath.getShortFileName());
			auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Physics convertor error : creating directory" << std::endl;
				return false;
			}
			auto fileName = cookingTask->dataSet->filePath.getShortFileName() + "_bullet_convex.phage";
			auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder() + fileName;

			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			btDefaultSerializer	serializer;
			serializer.startSerialization();
			cookingTask->convexShape->serializeSingleShape(&serializer);
			serializer.finishSerialization();
			ofs.write((const char *) (serializer.getBufferPointer()), serializer.getCurrentBufferSize());
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		}
		return true;
	}

	static bool SaveBulletConcave(std::shared_ptr<CookingTask> cookingTask)
	{
		if (cookingTask->concaveShape)
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : saving concave shape for " + cookingTask->dataSet->filePath.getShortFileName());
			auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder());

			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Physics convertor error : creating directory" << std::endl;
				return false;
			}
			auto fileName = cookingTask->dataSet->filePath.getShortFileName() + "_bullet_concave.phage";
			auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder() + fileName;

			std::ofstream ofs(name, std::ios::trunc | std::ios::binary);
			btDefaultSerializer	serializer;
			serializer.startSerialization();
			cookingTask->concaveShape->serializeSingleShape(&serializer);
			serializer.finishSerialization();
			ofs.write((const char *) (serializer.getBufferPointer()), serializer.getCurrentBufferSize());
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		}
		return true;
	}

	static bool SavePhysXConvex(std::shared_ptr<CookingTask> cookingTask)
	{
		// TO_DO
		return true;
	}

	static bool SavePhysXConcave(std::shared_ptr<CookingTask> cookingTask)
	{
		// TO_DO
		return true;
	}

	bool PhysicsLoader::save(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->dataSet->loadPhysic)
			return true;
		return SaveBulletConvex(cookingTask) && SaveBulletConcave(cookingTask) && SavePhysXConvex(cookingTask) && SaveBulletConcave(cookingTask);
	}

	namespace Private
	{
		template <typename Tag>
		class FriendshipResult
		{
		public:
			// Type Aliases
			using Type = typename Tag::Type;

			// Static Attributes
			static Type Pointer;

			// Constructors
			FriendshipResult(void) = delete;

			FriendshipResult(const FriendshipResult &) = delete;

			// Assignment Operators
			FriendshipResult &operator=(const FriendshipResult &) = delete;

			// Destructor
			~FriendshipResult(void) = delete;
		};

		// Static Attributes
		template <typename Tag>
		typename FriendshipResult<Tag>::Type FriendshipResult<Tag>::Pointer;

		template <typename Tag, typename Tag::Type MagicPointer>
		class Friendship final : private FriendshipResult<Tag>
		{
		private:
			// Inner Classes
			class Filler final
			{
			public:
				// Constructors
				inline Filler(void);

				Filler(const Filler &) = delete;

				// Assignment Operators
				Filler &operator=(const Filler &) = delete;

				// Destructor
				~Filler(void) = default;
			};

			// Static Attributes
			static Filler FillerObject;

		public:
			// Constructors
			Friendship(void) = delete;

			Friendship(const Friendship &) = delete;

			// Assignment Operators
			Friendship &operator=(const Friendship &) = delete;

			// Destructor
			~Friendship(void) = delete;
		};

		// Constructors
		template <typename Tag, typename Tag::Type MagicPointer>
		inline Friendship<Tag, MagicPointer>::Filler::Filler(void)
		{
			FriendshipResult<Tag>::Pointer = MagicPointer;
		}

		// Static Attributes
		template <typename Tag, typename Tag::Type Pointer>
		typename Friendship<Tag, Pointer>::Filler Friendship<Tag, Pointer>::FillerObject;

		// Access Function
		template <typename AccessPolicy, class ClassType>
		inline auto ForceAccess(const ClassType *instance) -> typename std::add_lvalue_reference<decltype(instance->*Private::FriendshipResult<AccessPolicy>::Pointer)>::type
		{
			assert(instance != nullptr && "Invalid instance");
			return instance->*Private::FriendshipResult<AccessPolicy>::Pointer;
		}

		template <typename AccessPolicy, class ClassType>
		inline auto ForceAccess(ClassType *instance) -> typename std::add_lvalue_reference<decltype(instance->*Private::FriendshipResult<AccessPolicy>::Pointer)>::type
		{
			assert(instance != nullptr && "Invalid instance");
			return instance->*Private::FriendshipResult<AccessPolicy>::Pointer;
		}

		template <typename AccessPolicy, class ClassType>
		inline auto ForceAccess(const ClassType &instance) -> typename std::add_lvalue_reference<decltype(instance.*Private::FriendshipResult<AccessPolicy>::Pointer)>::type
		{
			return instance.*Private::FriendshipResult<AccessPolicy>::Pointer;
		}

		template <typename AccessPolicy, class ClassType>
		inline auto ForceAccess(ClassType &instance) -> typename std::add_lvalue_reference<decltype(instance.*Private::FriendshipResult<AccessPolicy>::Pointer)>::type
		{
			return instance.*Private::FriendshipResult<AccessPolicy>::Pointer;
		}

		// Give access to the private btConvexHullShape::m_unscaledPoints
		struct UnscaledPoints final
		{
			using Type = btAlignedObjectArray<btVector3> btConvexHullShape::*;
		};

		template
		class Friendship<UnscaledPoints, &btConvexHullShape::m_unscaledPoints>;
	}

	static bool LoadBulletConvex(std::shared_ptr<CookingTask> cookingTask)
	{
		if (cookingTask->dataSet->convex)
		{
			cookingTask->convexShape = std::make_shared<btConvexHullShape>();
			Private::ForceAccess<Private::UnscaledPoints>(*cookingTask->convexShape).clear();
			std::shared_ptr<MeshData> mesh = cookingTask->mesh;
			assert(mesh != nullptr && "Invalid mesh");
			if (mesh != nullptr)
			{
				for (SubMeshData &subMesh : mesh->subMeshs)
				{
					for (const glm::vec3 &position : subMesh.positions)
					{
						cookingTask->convexShape->addPoint(btVector3(position.x, position.y, position.z), false);
					}
				}
				cookingTask->convexShape->recalcLocalAabb();
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	static bool LoadBulletConcave(std::shared_ptr<CookingTask> cookingTask)
	{
		if (cookingTask->dataSet->concave)
		{
			std::shared_ptr<MeshData> mesh = cookingTask->mesh;
			assert(mesh != nullptr && "Invalid mesh");
			if (mesh != nullptr)
			{
				btTriangleMesh *triangleMesh = new btTriangleMesh(true, true);
				for (SubMeshData &subMesh : mesh->subMeshs)
				{
					std::vector<unsigned int>::const_iterator it = subMesh.indices.begin();
					std::vector<unsigned int>::const_iterator end = subMesh.indices.end();
					while (it != end)
					{
						const btVector3 firstVertex(subMesh.positions[*it].x, subMesh.positions[*it].y, subMesh.positions[*it].z);
						++it;
						const btVector3 secondVertex(subMesh.positions[*it].x, subMesh.positions[*it].y, subMesh.positions[*it].z);
						++it;
						const btVector3 thirdVertex(subMesh.positions[*it].x, subMesh.positions[*it].y, subMesh.positions[*it].z);
						++it;
						triangleMesh->addTriangle(firstVertex, secondVertex, thirdVertex, true);
					}
				}
				cookingTask->concaveShape = std::make_shared<btBvhTriangleMeshShape>(triangleMesh, true);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	static bool LoadPhysXConvex(std::shared_ptr<CookingTask> cookingTask)
	{
		// TO_DO
		return true;
	}

	static bool LoadPhysXConcave(std::shared_ptr<CookingTask> cookingTask)
	{
		// TO_DO
		return true;
	}

	bool PhysicsLoader::load(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->dataSet->loadPhysic)
			return true;
		auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : loading " + cookingTask->dataSet->filePath.getShortFileName());
		const bool returnValue = LoadBulletConvex(cookingTask) && LoadBulletConcave(cookingTask) && LoadPhysXConvex(cookingTask) && LoadPhysXConcave(cookingTask);
		Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		return returnValue;
	}
}