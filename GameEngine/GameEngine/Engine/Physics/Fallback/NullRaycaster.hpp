#pragma once

#include <Physics/RaycasterInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullRaycaster final : public RaycasterInterface
		{
		public:
			// Constructors
			NullRaycaster(void) = delete;

			NullRaycaster(WorldInterface *world);

			NullRaycaster(const NullRaycaster &) = delete;

			// Assignment Operators
			NullRaycaster &operator=(const NullRaycaster &) = delete;

		private:
			// Destructor
			~NullRaycaster(void) = default;

			// Inherited Methods
			bool raycast(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers) override final;

			std::vector<RaycastHit> raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance, LayerMask layers) override final;
		};
	}
}