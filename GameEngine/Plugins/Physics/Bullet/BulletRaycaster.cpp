#include "BulletRaycaster.hpp"

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
			// TO_DO
			return false;
		}

		std::vector<RaycastHit> BulletRaycaster::raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers)
		{
			// TO_DO
			return std::vector<RaycastHit>();
		}
	}
}