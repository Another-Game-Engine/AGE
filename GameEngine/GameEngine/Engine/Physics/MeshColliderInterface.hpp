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

			MeshColliderInterface(WorldInterface *world, const std::string &mesh, bool mustBeConvex, Private::GenericData *data);

			MeshColliderInterface(const MeshColliderInterface &) = delete;

			// Assignment Operators
			MeshColliderInterface &operator=(const MeshColliderInterface &) = delete;

			// Methods
			const std::string &getMesh(void) const;

			bool isConvex(void) const;

			void setAsConvex(void);

			bool isConcave(void) const;

			void setAsConcave(void);

			// Virtual Methods
			virtual void setMesh(const std::string &mesh);

			virtual void updateShape(void) = 0;

		protected:
			// Static Methods
			static bool IsConvexByDefault(void);

			// Destructor
			virtual ~MeshColliderInterface(void) = default;

		private:
			// Attributes
			std::string mesh;

			bool convex;

			// Inherited Methods
			ColliderType getColliderType(void) const override final;
		};
	}
}

#include "MeshColliderInterface.inl"