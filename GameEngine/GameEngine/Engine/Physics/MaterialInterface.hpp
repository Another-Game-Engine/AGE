#pragma once

namespace AGE
{
	namespace Physics
	{
		class RigidBodyInterface;
		class ColliderInterface;
		class WorldInterface;

		class MaterialInterface
		{
			// Friendships
			friend WorldInterface;
		
		public:
			// Constructors
			MaterialInterface(void) = delete;

			MaterialInterface(ColliderInterface *collider);

			MaterialInterface(const MaterialInterface &) = delete;

			// Assignment Operators
			MaterialInterface &operator=(const MaterialInterface &) = delete;

			// Methods
			ColliderInterface *getCollider(void);

			const ColliderInterface *getCollider(void) const;

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
			ColliderInterface *collider = nullptr;
		};
	}
}

#include "MaterialInterface.inl"