#pragma once

#include <Physics/BoxColliderInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullBoxCollider final : public BoxColliderInterface
		{
		public:
			// Constructors
			NullBoxCollider(void) = delete;

			NullBoxCollider(WorldInterface *world);

			NullBoxCollider(const NullBoxCollider &) = delete;

			// Assignment Operators
			NullBoxCollider &operator=(const NullBoxCollider &) = delete;

		protected:
			// Destructor
			~NullBoxCollider(void) = default;
		};
	}
}