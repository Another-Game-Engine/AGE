#pragma once

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BoxColliderInterface : public virtual ColliderInterface
		{
		public:
			// Constructors
			BoxColliderInterface(void) = delete;

			BoxColliderInterface(WorldInterface *world, Private::GenericData *data);

			BoxColliderInterface(const BoxColliderInterface &) = delete;

			// Assignment Operators
			BoxColliderInterface &operator=(const BoxColliderInterface &) = delete;

			// Virtual Methods
			virtual void setCenter(const glm::vec3 &center) = 0;

			virtual glm::vec3 getCenter(void) const = 0;

			virtual void setSize(const glm::vec3 &size) = 0;

			virtual glm::vec3 getSize(void) const = 0;

		protected:
			// Destructor
			virtual ~BoxColliderInterface(void) = default;

		private:
			// Inherited Methods
			ColliderType getColliderType(void) const override final;
		};
	}
}

#include "BoxColliderInterface.inl"