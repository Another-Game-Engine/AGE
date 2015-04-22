#pragma once

#include "ColliderInterface.hpp"
#include "PhysXPhysics.hpp"
#include "PhysXMaterial.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXRigidBody;

		class PhysXCollider : public virtual ColliderInterface
		{
			// Friendships
			friend PhysXRigidBody;

		public:
			// Constructors
			PhysXCollider(void) = delete;

			PhysXCollider(WorldInterface *world, Private::GenericData *data, physx::PxShape *shape);

			PhysXCollider(const PhysXCollider &) = delete;

			// Assignment Operators
			PhysXCollider &operator=(const PhysXCollider &) = delete;

		protected:
			// Destructor
			virtual ~PhysXCollider(void);

			// Methods
			physx::PxShape *getShape(void);

			const physx::PxShape *getShape(void) const;

		private:
			// Attributes
			physx::PxShape *shape = nullptr;

			PhysXRigidBody *rigidBody = nullptr;

			// Inherited Methods
			void setAsTrigger(bool mustBeATrigger) override final;

			bool isATrigger(void) const override final;

			void setFilterGroup(FilterGroup group) override final;

			FilterGroup getFilterGroup(void) const override final;
		};
	}
}