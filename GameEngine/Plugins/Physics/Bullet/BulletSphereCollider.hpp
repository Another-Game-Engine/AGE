#pragma once

#include "SphereColliderInterface.hpp"
#include "BulletCollider.hpp"

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class BulletSphereCollider final : public SphereColliderInterface, public BulletCollider
		{
			// Friendships
			friend ObjectPool < BulletSphereCollider > ;

		public:
			// Constructors
			BulletSphereCollider(void) = delete;

			BulletSphereCollider(WorldInterface *world, Private::GenericData *data);

			BulletSphereCollider(const BulletSphereCollider &) = delete;

			// Assignment Operators
			BulletSphereCollider &operator=(const BulletSphereCollider &) = delete;

		private:
			// Destructor
			~BulletSphereCollider(void) = default;

			// Inherited Methods
			void setCenter(const glm::vec3 &center) override final;

			glm::vec3 getCenter(void) const override final;

			void setRadius(float radius) override final;

			float getRadius(void) const override final;
		};
	}
}

#pragma warning(pop)