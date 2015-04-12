#pragma once

#include "BoxColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletBoxCollider final : public BoxColliderInterface
		{
		public:
			// Constructors
			BulletBoxCollider(void) = delete;

			BulletBoxCollider(WorldInterface *world);

			BulletBoxCollider(const BulletBoxCollider &) = delete;

			// Assignment Operators
			BulletBoxCollider &operator=(const BulletBoxCollider &) = delete;

		protected:
			// Destructor
			~BulletBoxCollider(void) = default;
		};
	}
}