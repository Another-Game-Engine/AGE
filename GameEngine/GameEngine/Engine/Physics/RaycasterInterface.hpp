#pragma once

#include <vector>
#include <limits>

#include <glm/glm.hpp>

#include "RaycastHit.hpp"
#include "FilterGroup.hpp"

namespace AGE
{
	namespace Physics
	{
		class WorldInterface;

		class RaycasterInterface
		{
			// Friendships
			friend WorldInterface;

		public:
			// Constructors
			RaycasterInterface(void) = delete;

			RaycasterInterface(WorldInterface *world);

			RaycasterInterface(const RaycasterInterface &) = delete;

			// Assignment Operators
			RaycasterInterface &operator=(const RaycasterInterface &) = delete;

			// Methods
			WorldInterface *getWorld(void);

			const WorldInterface *getWorld(void) const;

			virtual bool raycast(const glm::vec3 &origin, const glm::vec3 &direction, float distance = std::numeric_limits<float>::max(), LayerMask layers = DefaultLayerMask) = 0;

			virtual std::vector<RaycastHit> raycastAll(const glm::vec3 &origin, const glm::vec3 &direction, float distance = std::numeric_limits<float>::max(), LayerMask layers = DefaultLayerMask) = 0;

		protected:
			// Attributes
			WorldInterface *world = nullptr;

			// Destructor
			virtual ~RaycasterInterface(void) = default;
		};
	}
}

#include "RaycasterInterface.inl"