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
			const physx::PxHitFlags outputFlags = physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eMESH_BOTH_SIDES;
			physx::PxQueryFilterData filterData;
			filterData.flags = physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC | physx::PxQueryFlag::eANY_HIT;
			filterData.data.word0 = static_cast<physx::PxU32>(layers);
			return static_cast<PhysXWorld *>(getWorld())->getScene()->raycast(rayOrigin, rayDirection, distance, hit, outputFlags, filterData);
		}

		std::vector<RaycastHit> PhysXRaycaster::raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers)
		{
			const physx::PxVec3 rayOrigin(origin.x, origin.y, origin.z);
			const glm::vec3 normalizedDirection = glm::normalize(direction);
			const physx::PxVec3 rayDirection = physx::PxVec3(normalizedDirection.x, normalizedDirection.y, normalizedDirection.z);
			physx::PxRaycastBuffer hit;
			const physx::PxHitFlags outputFlags = physx::PxHitFlag::eDEFAULT | physx::PxHitFlag::eMESH_BOTH_SIDES;
			physx::PxQueryFilterData filterData;
			filterData.flags = physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC;
			filterData.data.word0 = static_cast<physx::PxU32>(layers);
			std::vector<RaycastHit> results;
			if (static_cast<PhysXWorld *>(getWorld())->getScene()->raycast(rayOrigin, rayDirection, distance, hit, outputFlags, filterData))
			{
				for (physx::PxU32 index = 0, numberOfTouches = hit.getNbTouches(); index < numberOfTouches; ++index)
				{
					const physx::PxRaycastHit &raycastHit = hit.getTouch(index);
					PhysXCollider *collider = static_cast<PhysXCollider *>(raycastHit.actor->userData);
					if ((layers & static_cast<LayerMask>(1 << static_cast<std::uint32_t>(collider->getFilterGroup()))) != LayerMask::None)
					{
						RaycastHit result;
						result.hitEntity = collider->getData()->entity;
						result.distanceFromRayOrigin = raycastHit.distance;
						result.impactNormal = glm::vec3(raycastHit.normal.x, raycastHit.normal.y, raycastHit.normal.z);
						result.impactPoint = glm::vec3(raycastHit.position.x, raycastHit.position.y, raycastHit.position.z);
						results.emplace_back(std::move(result));
					}
				}
			}
			return std::move(results);
		}
	}
}