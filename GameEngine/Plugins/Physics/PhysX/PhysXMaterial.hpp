#pragma once

#include <PxPhysicsAPI.h>

#include "MaterialInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class PhysXBoxCollider;
		class PhysXCapsuleCollider;
		class PhysXMeshCollider;
		class PhysXSphereCollider;
		class PhysXWorld;

		class PhysXMaterial final : public MaterialInterface
		{
			// Friendships
			friend PhysXBoxCollider;
			
			friend PhysXCapsuleCollider;
			
			friend PhysXMeshCollider;

			friend PhysXSphereCollider;

			friend ObjectPool < PhysXMaterial >;

		public:
			// Constructors
			PhysXMaterial(void) = delete;

			PhysXMaterial(PhysXWorld *world, ColliderInterface *collider);

			PhysXMaterial(const PhysXMaterial &) = delete;

			// Assignment Operators
			PhysXMaterial &operator=(const PhysXMaterial &) = delete;

		private:
			// Attributes
			physx::PxMaterial *material = nullptr;

			// Destructor
			~PhysXMaterial(void);

			// Inherited Methods
			void setStaticFriction(float staticFriction) override final;

			float getStaticFriction(void) const override final;

			void setDynamicFriction(float dynamicFriction) override final;

			float getDynamicFriction(void) const override final;

			void setRestitution(float restitution) override final;

			float getRestitution(void) const override final;
		};
	}
}