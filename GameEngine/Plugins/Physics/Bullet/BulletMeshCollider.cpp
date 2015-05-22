#pragma once

#include "BulletMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
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

		// Static Methods
		btTriangleMesh *BulletMeshCollider::CreateTriangleMesh(std::shared_ptr<MeshInstance> mesh)
		{
			assert(mesh != nullptr && "Invalid mesh");
			if (mesh != nullptr)
			{
				btTriangleMesh *triangleMesh = new btTriangleMesh(true, true);
				for (SubMeshData &subMesh : mesh->meshData->subMeshs)
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
				return triangleMesh;
			}
			else
			{
				return nullptr;
			}
		}

		// Constructors
		BulletMeshCollider::BulletMeshCollider(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, mesh, IsConvexByDefault(), data), BulletCollider(world, data,
			IsConvexByDefault() ?
			static_cast<btCollisionShape *>(new btConvexHullShape()) :
			static_cast<btCollisionShape *>(new btBvhTriangleMeshShape(BulletMeshCollider::CreateTriangleMesh(mesh), true)))
		{
			if (isConvex())
			{
				setMesh(mesh);
			}
		}

		// Inherited Methods
		void BulletMeshCollider::setMesh(std::shared_ptr<MeshInstance> mesh)
		{
			MeshColliderInterface::setMesh(mesh);
			if (isConvex())
			{
				btConvexHullShape *shape = static_cast<btConvexHullShape *>(getShape());
				Private::ForceAccess<Private::UnscaledPoints>(shape).clear();
				assert(mesh != nullptr && "Invalid mesh");
				if (mesh != nullptr)
				{
					for (SubMeshData &subMesh : mesh->meshData->subMeshs)
					{
						for (const glm::vec3 &position : subMesh.positions)
						{
							shape->addPoint(btVector3(position.x, position.y, position.z), false);
						}
					}
					shape->recalcLocalAabb();
				}
			}
			else
			{
				btCollisionShape *shape = static_cast<btCollisionShape *>(new btBvhTriangleMeshShape(BulletMeshCollider::CreateTriangleMesh(mesh), true));
				static_cast<BulletCollider *>(this)->updateShape(shape);
			}
		}

		void BulletMeshCollider::updateShape(void)
		{
			std::shared_ptr<MeshInstance> mesh = getMesh();
			assert(mesh != nullptr && "Invalid mesh");
			if (isConvex())
			{
				btCollisionShape *shape = static_cast<btCollisionShape *>(new btConvexHullShape());
				static_cast<BulletCollider *>(this)->updateShape(shape);
				setMesh(mesh);
			}
			else
			{
				btCollisionShape *shape = static_cast<btCollisionShape *>(new btBvhTriangleMeshShape(BulletMeshCollider::CreateTriangleMesh(mesh), true));
				static_cast<BulletCollider *>(this)->updateShape(shape);
			}
		}
	}
}