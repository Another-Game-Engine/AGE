#include "NullRaycaster.hpp"

namespace AGE
{
	namespace Physics
	{
		// Constructors
		NullRaycaster::NullRaycaster(WorldInterface *world)
			: RaycasterInterface(world)
		{
			return;
		}

		// Inherited Methods
		bool NullRaycaster::raycast(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers)
		{
			return false;
		}

		std::vector<RaycastHit> NullRaycaster::raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers)
		{
			return std::vector<RaycastHit>();
		}
	}
}