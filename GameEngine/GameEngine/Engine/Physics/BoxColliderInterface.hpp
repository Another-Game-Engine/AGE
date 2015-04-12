#pragma once

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class BoxColliderInterface : public ColliderInterface
		{
		public:
			// Constructors
			BoxColliderInterface(void) = delete;

			BoxColliderInterface(WorldInterface *world);

			BoxColliderInterface(const BoxColliderInterface &) = delete;

			// Assignment Operators
			BoxColliderInterface &operator=(const BoxColliderInterface &) = delete;

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