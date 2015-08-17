#include "PhysXWorld.hpp"
#include "PhysXRaycaster.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		PhysXRaycaster::PhysXRaycaster(WorldInterface *world)
			: RaycasterInterface(world)
		{
			return;
		}

		// Inherited Methods
		bool PhysXRaycaster::raycast(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers)
		{
			const physx::PxVec3 rayOrigin(origin.x, origin.y, origin.z);
			const glm::vec3 normalizedDirection = glm::normalize(direction);
			const physx::PxVec3 rayDirection = physx::PxVec3(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
			physx::PxRaycastBuffer hit;
			const physx::PxHitFlags outputFlags = physx::PxHitFlag::eNORMAL;
			physx::PxQueryFilterData filterData;
			filterData.data.word0 = static_cast<std::uint32_t>(layers);
			return static_cast<PhysXWorld *>(getWorld())->getScene()->raycast(rayOrigin, rayDirection, distance, hit, outputFlags, filterData);
		}

		std::vector<RaycastHit> PhysXRaycaster::raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers)
		{
			const physx::PxVec3 rayOrigin(origin.x, origin.y, origin.z);
			const glm::vec3 normalizedDirection = glm::normalize(direction);
			const physx::PxVec3 rayDirection = physx::PxVec3(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
			physx::PxRaycastBuffer hit;
			const physx::PxHitFlags outputFlags = physx::PxHitFlag::eNORMAL;
			physx::PxQueryFilterData filterData;
			filterData.data.word0 = static_cast<std::uint32_t>(layers);
			std::vector<RaycastHit> results;
			if (static_cast<PhysXWorld *>(getWorld())->getScene()->raycast(rayOrigin, rayDirection, distance, hit, outputFlags, filterData))
			{
				for (physx::PxU32 index = 0, numberOfTouches = hit.getNbTouches(); index < numberOfTouches; ++index)
				{
					const physx::PxRaycastHit &raycastHit = hit.getTouch(index);
					PhysXCollider *collider = static_cast<PhysXCollider *>(raycastHit.actor->userData);
					RaycastHit result;
					result.hitEntity = collider->getData()->entity;
					result.distanceFromRayOrigin = raycastHit.distance;
					result.impactNormal = glm::vec3(raycastHit.normal.x, raycastHit.normal.y, raycastHit.normal.z);
					result.impactPoint = glm::vec3(raycastHit.position.x, raycastHit.position.y, raycastHit.position.z);
					results.emplace_back(std::move(result));
				}
			}
			return std::move(results);
		}
	}
}