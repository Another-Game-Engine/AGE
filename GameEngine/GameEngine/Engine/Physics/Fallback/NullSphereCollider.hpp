#pragma once

#include <Physics/SphereColliderInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullSphereCollider final : public SphereColliderInterface
		{
		public:
			// Constructors
			NullSphereCollider(void) = delete;

			NullSphereCollider(WorldInterface *world);

			NullSphereCollider(const NullSphereCollider &) = delete;

			// Assignment Operators
			NullSphereCollider &operator=(const NullSphereCollider &) = delete;

		protected:
			// Destructor
			~NullSphereCollider(void) = default;
		};
	}
}