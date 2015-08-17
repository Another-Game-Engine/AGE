#pragma once

#include "RaycasterInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXRaycaster final : public RaycasterInterface
		{
		public:
			// Constructors
			PhysXRaycaster(void) = delete;

			PhysXRaycaster(WorldInterface *world);

			PhysXRaycaster(const PhysXRaycaster &) = delete;

			// Assignment Operators
			PhysXRaycaster &operator=(const PhysXRaycaster &) = delete;

		private:
			// Destructor
			~PhysXRaycaster(void) = default;

			// Inherited Methods
			bool raycast(const glm::vec3 &origin, const glm::vec3 &direction, float distance = std::numeric_limits<float>::max(), LayerMask layers = Physics::DefaultLayerMask) override final;

			std::vector<RaycastHit> raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance = std::numeric_limits<float>::max(), LayerMask layers = Physics::DefaultLayerMask) override final;
		};
	}
}