#pragma once

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class CapsuleColliderInterface : public ColliderInterface
		{
		public:
			// Constructors
			CapsuleColliderInterface(void) = delete;

			CapsuleColliderInterface(WorldInterface *world);

			CapsuleColliderInterface(const CapsuleColliderInterface &) = delete;

			// Assignment Operators
			CapsuleColliderInterface &operator=(const CapsuleColliderInterface &) = delete;

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