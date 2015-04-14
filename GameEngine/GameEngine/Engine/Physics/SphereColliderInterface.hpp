#pragma once

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class SphereColliderInterface : public virtual ColliderInterface
		{
		public:
			// Constructors
			SphereColliderInterface(void) = delete;

			SphereColliderInterface(WorldInterface *world, Private::GenericData *data);

			SphereColliderInterface(const SphereColliderInterface &) = delete;

			// Assignment Operators
			SphereColliderInterface &operator=(const SphereColliderInterface &) = delete;

			// Virtual Methods
			virtual void setCenter(const glm::vec3 &center) = 0;

			virtual glm::vec3 getCenter(void) const = 0;

			virtual void setRadius(float radius) = 0;

			virtual float getRadius(void) const = 0;

		protected:
			// Destructor
			virtual ~SphereColliderInterface(void) = default;

		private:
			// Inherited Methods
			ColliderType getColliderType(void) const override final;
		};
	}
}

#include "SphereColliderInterface.inl"