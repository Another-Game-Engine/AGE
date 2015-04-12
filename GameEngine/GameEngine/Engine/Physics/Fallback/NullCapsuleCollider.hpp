#pragma once

#include <Physics/CapsuleColliderInterface.hpp>

namespace AGE
{
	namespace Physics
	{
		class NullCapsuleCollider final : public CapsuleColliderInterface
		{
		public:
			// Constructors
			NullCapsuleCollider(void) = delete;

			NullCapsuleCollider(WorldInterface *world);

			NullCapsuleCollider(const NullCapsuleCollider &) = delete;

			// Assignment Operators
			NullCapsuleCollider &operator=(const NullCapsuleCollider &) = delete;

		protected:
			// Destructor
			~NullCapsuleCollider(void) = default;
		};
	}
}