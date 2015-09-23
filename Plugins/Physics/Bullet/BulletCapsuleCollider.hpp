#pragma once

#include "CapsuleColliderInterface.hpp"
#include "BulletCollider.hpp"

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class BulletCapsuleCollider final : public CapsuleColliderInterface, public BulletCollider
		{
			// Friendships
			friend ObjectPool < BulletCapsuleCollider > ;

		public:
			// Constructors
			BulletCapsuleCollider(void) = delete;

			BulletCapsuleCollider(WorldInterface *world, Private::GenericData *data);

			BulletCapsuleCollider(const BulletCapsuleCollider &) = delete;

			// Assignment Operators
			BulletCapsuleCollider &operator=(const BulletCapsuleCollider &) = delete;

		private:
			// Attributes
			glm::vec3 center;

			// Destructor
			~BulletCapsuleCollider(void) = default;

			// Inherited Methods
			void setCenter(const glm::vec3 &center) override final;

			glm::vec3 getCenter(void) const override final;

			void setHeight(float height) override final;

			float getHeight(void) const override final;

			void setRadius(float radius) override final;

			float getRadius(void) const override final;
		};
	}
}

#pragma warning(pop)