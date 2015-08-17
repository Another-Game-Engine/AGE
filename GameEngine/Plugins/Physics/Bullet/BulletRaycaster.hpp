#pragma once

#include "RaycasterInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletRaycaster final : public RaycasterInterface
		{
		public:
			// Constructors
			BulletRaycaster(void) = delete;

			BulletRaycaster(WorldInterface *world);

			BulletRaycaster(const BulletRaycaster &) = delete;

			// Assignment Operators
			BulletRaycaster &operator=(const BulletRaycaster &) = delete;

		private:
			// Destructor
			~BulletRaycaster(void) = default;

			// Inherited Methods
			bool raycast(const glm::vec3 &origin, const glm::vec3 &direction, float distance = std::numeric_limits<float>::max(), LayerMask layers = Physics::DefaultLayerMask) override final;

			std::vector<RaycastHit> raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance = std::numeric_limits<float>::max(), LayerMask layers = Physics::DefaultLayerMask) override final;
		};
	}
}