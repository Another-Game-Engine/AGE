#pragma once

#include "BulletMeshCollider.hpp"

namespace AGE
{
	namespace Physics
	{
		namespace Private
		{
			// Access a Private Member without the need of friendship (Used to clear the Bullet shape array)
			template <typename Tag>
			struct Result
			{
				using Type = typename Tag::Type;

				static Type Pointer;
			};

			template <typename Tag>
			typename Result<Tag>::Type Result<Tag>::Pointer;

			template <typename Tag, typename Tag::Type MagicPointer>
			struct Robber final : private Result<Tag>
			{
				struct Filler final
				{
					Filler(void)
					{
						Result<Tag>::Pointer = MagicPointer;
					}
				};

				static Filler FillerObject;
			};

			template <typename Tag, typename Tag::Type Pointer>
			typename Robber<Tag, Pointer>::Filler Robber<Tag, Pointer>::FillerObject;

			// Rob the m_unscaledPoints data member
			struct PointRobber final
			{
				using Type = btAlignedObjectArray<btVector3> btConvexHullShape::*;
			};

			template
			struct Robber<PointRobber, &btConvexHullShape::m_unscaledPoints>;
		}

		// Constructors
		BulletMeshCollider::BulletMeshCollider(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, Private::GenericData *data)
			: ColliderInterface(world, data), MeshColliderInterface(world, mesh, data), BulletCollider(world, data, new btConvexHullShape())
		{
			setMesh(mesh);
		}

		// Inherited Methods
		void BulletMeshCollider::setMesh(std::shared_ptr<MeshInstance> mesh)
		{
			MeshColliderInterface::setMesh(mesh);
			btConvexHullShape *shape = static_cast<btConvexHullShape *>(getShape());
			(shape->*Private::Result<Private::PointRobber>::Pointer).clear();
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
	}
}