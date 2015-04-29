#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BulletRigidBody;

		class BulletCollider : public virtual ColliderInterface
		{
			// Friendship
			friend BulletRigidBody;

		public:
			// Constructors
			BulletCollider(void) = delete;

			BulletCollider(WorldInterface *world, Private::GenericData *data, btCollisionShape *collisionShape);

			BulletCollider(const BulletCollider &) = delete;

			// Assignment Operators
			BulletCollider &operator=(const BulletCollider &) = delete;

			// Methods
			void updateShape(btCollisionShape *shape);

		protected:
			// Destructor
			virtual ~BulletCollider(void);

			// Methods
			btCollisionShape *getShape(void);

			const btCollisionShape *getShape(void) const;

		private:
			// Attributes
			btCollisionShape *collisionShape = nullptr;

			BulletRigidBody *rigidBody = nullptr;

			FilterGroup filterGroup = FilterGroup::GroupI;

			// Inherited Methods
			void setAsTrigger(bool mustBeATrigger) override final;

			bool isATrigger(void) const override final;

			void setFilterGroup(FilterGroup group) override final;

			FilterGroup getFilterGroup(void) const override final;

			void scale(const glm::vec3 &scaling) override final;
		};
	}
}