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
#include "Physics/PhysicsInterface.hpp"
#include "../Plugins/Physics/PhysX/PhysXPhysics.hpp"
#include "../Plugins/Physics/PhysX/PhysXWorld.hpp"

namespace AGE
{
	static bool SaveBulletConvex(std::shared_ptr<CookingTask> cookingTask)
	{
		if (cookingTask->convexShape)
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : saving bullet convex shape for " + cookingTask->dataSet->filePath.getShortFileName());
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
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : saving bullet concave shape for " + cookingTask->dataSet->filePath.getShortFileName());
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
		if (!cookingTask->physxConvexShapes.empty())
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : saving physx convex shape for " + cookingTask->dataSet->filePath.getShortFileName());
			auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder());
			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Physics convertor error : creating directory" << std::endl;
				return false;
			}
			auto fileName = cookingTask->dataSet->filePath.getShortFileName() + "_physx_convex.phage";
			auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder() + fileName;
			physx::PxDefaultFileOutputStream stream(name.c_str());
			physx::PxSerializationRegistry *registry = physx::PxSerialization::createSerializationRegistry(PxGetPhysics());
			physx::PxCollection *collection = PxCreateCollection();
			for (physx::PxConvexMesh *convexShape : cookingTask->physxConvexShapes)
			{
				collection->add(*convexShape);
			}
			if (collection->getNbObjects() == 0)
			{
				for (physx::PxTriangleMesh *concaveShape : cookingTask->physxConcaveShapes)
				{
					collection->add(*concaveShape);
				}
			}
			physx::PxSerialization::complete(*collection, *registry);
			physx::PxSerialization::serializeCollectionToBinary(stream, *collection, *registry);
			collection->release();
			registry->release();
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		}
		return true;
	}

	static bool SavePhysXConcave(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->physxConcaveShapes.empty())
		{
			auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : saving physx concave shape for " + cookingTask->dataSet->filePath.getShortFileName());
			auto folderPath = std::tr2::sys::path(cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder());
			if (!std::tr2::sys::exists(folderPath) && !std::tr2::sys::create_directories(folderPath))
			{
				Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
				std::cerr << "Physics convertor error : creating directory" << std::endl;
				return false;
			}
			auto fileName = cookingTask->dataSet->filePath.getShortFileName() + "_physx_concave.phage";
			auto name = cookingTask->serializedDirectory.path().directory_string() + "\\" + cookingTask->dataSet->filePath.getFolder() + fileName;
			physx::PxDefaultFileOutputStream stream(name.c_str());
			physx::PxSerializationRegistry *registry = physx::PxSerialization::createSerializationRegistry(PxGetPhysics());
			physx::PxCollection *collection = PxCreateCollection();
			for (physx::PxTriangleMesh *concaveShape : cookingTask->physxConcaveShapes)
			{
				collection->add(*concaveShape);
			}
			if (collection->getNbObjects() == 0)
			{
				for (physx::PxConvexMesh *convexShape : cookingTask->physxConvexShapes)
				{
					collection->add(*convexShape);
				}
			}
			physx::PxSerialization::complete(*collection, *registry);
			physx::PxSerialization::serializeCollectionToBinary(stream, *collection, *registry);
			collection->release();
			registry->release();
			Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		}
		return true;
	}

	bool PhysicsLoader::save(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->dataSet->loadPhysic)
			return true;
		bool returnValue = SaveBulletConvex(cookingTask);
		returnValue &= SaveBulletConcave(cookingTask);
		returnValue &= SavePhysXConvex(cookingTask);
		returnValue &= SavePhysXConcave(cookingTask);
		return returnValue;
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
				btTriangleMesh *triangleMesh = new btTriangleMesh();
				std::size_t indiceNb = 0;
				std::size_t verticeNb = 0;
				for (SubMeshData &subMesh : mesh->subMeshs)
				{
					indiceNb += subMesh.indices.size();
					verticeNb += subMesh.positions.size();
				}
				triangleMesh->preallocateIndices(static_cast<int>(indiceNb));
				triangleMesh->preallocateVertices(static_cast<int>(verticeNb));
				for (std::size_t j = 0; j < mesh->subMeshs.size(); ++j)
				{
					auto &geo = mesh->subMeshs[j];
					for (std::size_t i = 0; i < geo.indices.size(); i += 3)
					{
						auto a = geo.positions[geo.indices[i]];
						auto b = geo.positions[geo.indices[i + 1]];
						auto c = geo.positions[geo.indices[i + 2]];
						triangleMesh->addTriangle(btVector3(a.x, a.y, a.z), btVector3(b.x, b.y, b.z), btVector3(c.x, c.y, c.z), false);
					}
				}
				cookingTask->concaveShape = std::make_shared<btBvhTriangleMeshShape>(triangleMesh, true);
				cookingTask->concaveShape->buildOptimizedBvh();
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
		if (cookingTask->dataSet->convex)
		{
			std::shared_ptr<MeshData> mesh = cookingTask->mesh;
			assert(mesh != nullptr && "Invalid mesh");
			if (mesh != nullptr)
			{
				physx::PxCooking *cooking = PxCreateCooking(PX_PHYSICS_VERSION, PxGetFoundation(), physx::PxCookingParams(physx::PxTolerancesScale()));
				for (SubMeshData &subMesh : mesh->subMeshs)
				{
					std::vector<physx::PxVec3> points;
					for (const glm::vec3 &position : subMesh.positions)
					{
						points.push_back(physx::PxVec3(position.x, position.y, position.z));
					}
					physx::PxConvexMeshDesc meshDesciption;
					meshDesciption.points.count = static_cast<physx::PxU32>(points.size());
					meshDesciption.points.stride = static_cast<physx::PxU32>(sizeof(physx::PxVec3));
					meshDesciption.points.data = static_cast<const void *>(&points[0]);
					meshDesciption.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eINFLATE_CONVEX;
					meshDesciption.vertexLimit = physx::PxU16(256);
					physx::PxDefaultMemoryOutputStream writeBuffer;
					const bool status = cooking->cookConvexMesh(meshDesciption, writeBuffer);
					assert(status && "Impossible to create convex shape");
					if (!status)
					{
						return false;
					}
					physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
					cookingTask->physxConvexShapes.push_back(PxGetPhysics().createConvexMesh(readBuffer));
				}
				cooking->release();
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	static bool LoadPhysXConcave(std::shared_ptr<CookingTask> cookingTask)
	{
		if (cookingTask->dataSet->concave)
		{
			std::shared_ptr<MeshData> mesh = cookingTask->mesh;
			assert(mesh != nullptr && "Invalid mesh");
			if (mesh != nullptr)
			{
				physx::PxCooking *cooking = PxCreateCooking(PX_PHYSICS_VERSION, PxGetFoundation(), physx::PxCookingParams(physx::PxTolerancesScale()));
				for (SubMeshData &subMesh : mesh->subMeshs)
				{
					std::vector<physx::PxVec3> points;
					std::vector<physx::PxU32> indices;
					for (const glm::vec3 &position : subMesh.positions)
					{
						points.push_back(physx::PxVec3(position.x, position.y, position.z));
					}
					for (unsigned int index : subMesh.indices)
					{
						indices.push_back(index);
					}
					physx::PxTriangleMeshDesc meshDesciption;
					meshDesciption.points.count = static_cast<physx::PxU32>(points.size());
					meshDesciption.points.stride = static_cast<physx::PxU32>(sizeof(physx::PxVec3));
					meshDesciption.points.data = static_cast<const void *>(&points[0]);
					meshDesciption.triangles.count = static_cast<physx::PxU32>(indices.size() / 3);
					meshDesciption.triangles.stride = static_cast<physx::PxU32>(3 * sizeof(physx::PxU32));
					meshDesciption.triangles.data = static_cast<const void *>(&indices[0]);
					physx::PxDefaultMemoryOutputStream writeBuffer;
					const bool status = cooking->cookTriangleMesh(meshDesciption, writeBuffer);
					assert(status && "Impossible to create concave shape");
					if (!status)
					{
						return false;
					}
					physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
					cookingTask->physxConcaveShapes.push_back(PxGetPhysics().createTriangleMesh(readBuffer));
				}
				cooking->release();
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	bool PhysicsLoader::load(std::shared_ptr<CookingTask> cookingTask)
	{
		if (!cookingTask->dataSet->loadPhysic)
			return true;
		auto tid = Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PushTask("PhysicsLoader : loading " + cookingTask->dataSet->filePath.getShortFileName());
		bool returnValue = LoadBulletConvex(cookingTask);
		returnValue &= LoadBulletConcave(cookingTask);
		returnValue &= LoadPhysXConvex(cookingTask);
		returnValue &= LoadPhysXConcave(cookingTask);
		Singleton<AGE::AE::ConvertorStatusManager>::getInstance()->PopTask(tid);
		return returnValue;
	}
}