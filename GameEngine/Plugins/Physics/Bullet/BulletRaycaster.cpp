#include "BulletRaycaster.hpp"
#include "BulletWorld.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		BulletRaycaster::BulletRaycaster(WorldInterface *world)
			: RaycasterInterface(world)
		{
			return;
		}

		// Inherited Methods
		bool BulletRaycaster::raycast(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers)
		{
			const btVector3 rayOrigin(origin.x, origin.y, origin.z);
			const glm::vec3 normalizedDirection = glm::normalize(direction);
			const btVector3 rayDirection(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
			const btVector3 rayEnd(rayOrigin + distance * rayDirection);
			btCollisionWorld::ClosestRayResultCallback rayCallback(rayOrigin, rayEnd);
			rayCallback.m_collisionFilterGroup = static_cast<short>(layers);
			rayCallback.m_collisionFilterMask = static_cast<short>(-1);
			static_cast<BulletWorld *>(getWorld())->getWorld()->rayTest(rayOrigin, rayEnd, rayCallback);
			return rayCallback.hasHit();
		}

		std::vector<RaycastHit> BulletRaycaster::raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers)
		{
			const btVector3 rayOrigin(origin.x, origin.y, origin.z);
			const glm::vec3 normalizedDirection = glm::normalize(direction);
			const btVector3 rayDirection(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
			const btVector3 rayEnd(rayOrigin + distance * rayDirection);
			btCollisionWorld::AllHitsRayResultCallback rayCallback(rayOrigin, rayEnd);
			rayCallback.m_collisionFilterGroup = static_cast<short>(layers);
			rayCallback.m_collisionFilterMask = static_cast<short>(-1);
			static_cast<BulletWorld *>(getWorld())->getWorld()->rayTest(rayOrigin, rayEnd, rayCallback);
			std::vector<RaycastHit> results;
			if (rayCallback.hasHit())
			{
				for (int index = 0; index < rayCallback.m_collisionObjects.size(); ++index)
				{
					BulletCollider *collider = static_cast<BulletCollider *>(rayCallback.m_collisionObjects[index]->getCollisionShape()->getUserPointer());
					RaycastHit result;
					result.hitEntity = collider->getData()->entity;
					result.distanceFromRayOrigin = static_cast<float>(rayOrigin.distance(rayCallback.m_hitPointWorld[index]));
					result.impactNormal = glm::vec3(rayCallback.m_hitNormalWorld[index].x(), rayCallback.m_hitNormalWorld[index].y(), rayCallback.m_hitNormalWorld[index].z());
					result.impactPoint = glm::vec3(rayCallback.m_hitPointWorld[index].x(), rayCallback.m_hitPointWorld[index].y(), rayCallback.m_hitPointWorld[index].z());
					results.emplace_back(std::move(result));
				}
			}
			return std::move(results);
		}
	}
}