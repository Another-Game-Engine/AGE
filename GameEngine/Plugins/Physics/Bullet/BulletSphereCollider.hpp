#pragma once

#include "SphereColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletSphereCollider final : public SphereColliderInterface
		{
		public:
			// Constructors
			BulletSphereCollider(void) = delete;

			BulletSphereCollider(WorldInterface *world);

			BulletSphereCollider(const BulletSphereCollider &) = delete;

			// Assignment Operators
			BulletSphereCollider &operator=(const BulletSphereCollider &) = delete;

		protected:
			// Destructor
			~BulletSphereCollider(void) = default;
		};
	}
}