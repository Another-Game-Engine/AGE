#pragma once

#include "ColliderInterface.hpp"

namespace AGE
{
	namespace Physics
	{
		class MeshColliderInterface : public virtual ColliderInterface
		{
		public:
			// Constructors
			MeshColliderInterface(void) = delete;

			MeshColliderInterface(WorldInterface *world, Private::GenericData *data);

			MeshColliderInterface(const MeshColliderInterface &) = delete;

			// Assignment Operators
			MeshColliderInterface &operator=(const MeshColliderInterface &) = delete;

		protected:
			// Destructor
			virtual ~MeshColliderInterface(void) = default;

		private:
			// Inherited Methods
			ColliderType getColliderType(void) const override final;
		};
	}
}

#include "MeshColliderInterface.inl"