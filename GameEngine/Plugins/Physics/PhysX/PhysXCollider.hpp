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

			PhysXCollider(WorldInterface *world, Private::GenericData *data, std::vector<physx::PxShape *> shapes);

			PhysXCollider(const PhysXCollider &) = delete;

			// Methods
			void updateShape(std::vector<physx::PxShape *> newShapes);

			// Assignment Operators
			PhysXCollider &operator=(const PhysXCollider &) = delete;

		protected:
			// Destructor
			virtual ~PhysXCollider(void);

			// Methods
			std::vector<physx::PxShape *> &getShapes(void);

			const std::vector<physx::PxShape *> &getShapes(void) const;

		private:
			// Attributes
			std::vector<physx::PxShape *> shapes;

			PhysXRigidBody *rigidBody = nullptr;

			// Inherited Methods
			void setAsTrigger(bool mustBeATrigger) override final;

			bool isATrigger(void) const override final;

			void setFilterGroup(FilterGroup group) override final;

			FilterGroup getFilterGroup(void) const override final;

			void setMaterial(const std::string &name) override final;

			void setPosition(const glm::vec3 &position) override final;

			void setRotation(const glm::quat &rotation) override final;
		};
	}
}