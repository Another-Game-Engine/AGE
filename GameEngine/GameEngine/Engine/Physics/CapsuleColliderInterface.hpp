#pragma once

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class CapsuleColliderInterface : public virtual ColliderInterface
		{
		public:
			// Constructors
			CapsuleColliderInterface(void) = delete;

			CapsuleColliderInterface(WorldInterface *world, Private::GenericData *data);

			CapsuleColliderInterface(const CapsuleColliderInterface &) = delete;

			// Assignment Operators
			CapsuleColliderInterface &operator=(const CapsuleColliderInterface &) = delete;

			// Virtual Methods
			virtual void setCenter(const glm::vec3 &center) = 0;

			virtual glm::vec3 getCenter(void) const = 0;

			virtual void setHeight(float height) = 0;

			virtual float getHeight(void) const = 0;

			virtual void setRadius(float radius) = 0;

			virtual float getRadius(void) const = 0;

		protected:
			// Destructor
			virtual ~CapsuleColliderInterface(void) = default;

		private:
			// Inherited Methods
			ColliderType getColliderType(void) const override final;
		};
	}
}

#include "CapsuleColliderInterface.inl"