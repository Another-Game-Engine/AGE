#pragma once

namespace AGE
{
	namespace Physics
	{
		class WorldInterface;
		class RigidBodyInterface;

		class MaterialInterface
		{
			// Friendships
			friend WorldInterface;

			friend RigidBodyInterface;
		
		public:
			// Constructors
			MaterialInterface(void) = default;

			MaterialInterface(const MaterialInterface &) = delete;

			// Assignment Operators
			MaterialInterface &operator=(const MaterialInterface &) = delete;

			// Methods
			RigidBodyInterface *getRigidBody(void);

			const RigidBodyInterface *getRigidBody(void) const;

			// Virtual Methods
			virtual void setStaticFriction(float staticFriction) = 0;

			virtual float getStaticFriction(void) const = 0;

			virtual void setDynamicFriction(float dynamicFriction) = 0;

			virtual float getDynamicFriction(void) const = 0;

			virtual void setRestitution(float restitution) = 0;

			virtual float getRestitution(void) const = 0;

		protected:
			// Destructor
			virtual ~MaterialInterface(void) = default;

		private:
			// Attributes
			RigidBodyInterface *rigidBody = nullptr;
		};
	}
}

#include "MaterialInterface.inl"