#pragma once

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class SphereColliderInterface : public ColliderInterface
		{
		public:
			// Constructors
			SphereColliderInterface(void) = delete;

			SphereColliderInterface(WorldInterface *world);

			SphereColliderInterface(const SphereColliderInterface &) = delete;

			// Assignment Operators
			SphereColliderInterface &operator=(const SphereColliderInterface &) = delete;

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