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

			MeshColliderInterface(WorldInterface *world, std::shared_ptr<MeshInstance> mesh, bool mustBeConvex, Private::GenericData *data);

			MeshColliderInterface(const MeshColliderInterface &) = delete;

			// Assignment Operators
			MeshColliderInterface &operator=(const MeshColliderInterface &) = delete;

			// Methods
			std::shared_ptr<MeshInstance> getMesh(void);

			std::shared_ptr<const MeshInstance> getMesh(void) const;

			bool isConvex(void) const;

			void setAsConvex(bool mustBeConvex);

			// Virtual Methods
			virtual void setMesh(std::shared_ptr<MeshInstance> mesh);

			virtual void updateShape(void) = 0;

		protected:
			// Static Methods
			static bool IsConvexByDefault(void);

			// Destructor
			virtual ~MeshColliderInterface(void) = default;

			// Attributes
			std::shared_ptr<MeshInstance> mesh;

		private:
			// Attributes
			bool convex;

			// Inherited Methods
			ColliderType getColliderType(void) const override final;
		};
	}
}

#include "MeshColliderInterface.inl"