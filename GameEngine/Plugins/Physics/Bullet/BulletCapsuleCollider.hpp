#pragma once

#include "CapsuleColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletCapsuleCollider final : public CapsuleColliderInterface
		{
		public:
			// Constructors
			BulletCapsuleCollider(void) = delete;

			BulletCapsuleCollider(WorldInterface *world);

			BulletCapsuleCollider(const BulletCapsuleCollider &) = delete;

			// Assignment Operators
			BulletCapsuleCollider &operator=(const BulletCapsuleCollider &) = delete;

		protected:
			// Destructor
			~BulletCapsuleCollider(void) = default;
		};
	}
}