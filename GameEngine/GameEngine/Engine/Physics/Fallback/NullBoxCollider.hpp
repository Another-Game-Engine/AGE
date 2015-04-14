#pragma once

#include <Physics/BoxColliderInterface.hpp>
#include <Physics/Fallback/NullCollider.hpp>

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class NullBoxCollider final : public BoxColliderInterface, public NullCollider
		{
			// Friendships
			friend ObjectPool < NullBoxCollider >;

		public:
			// Constructors
			NullBoxCollider(void) = delete;

			NullBoxCollider(WorldInterface *world, Private::GenericData *data);

			NullBoxCollider(const NullBoxCollider &) = delete;

			// Assignment Operators
			NullBoxCollider &operator=(const NullBoxCollider &) = delete;

		private:
			// Destructor
			~NullBoxCollider(void) = default;

			// Inherited Methods
			void setCenter(const glm::vec3 &center) override final;

			glm::vec3 getCenter(void) const override final;

			void setSize(const glm::vec3 &size) override final;

			glm::vec3 getSize(void) const override final;
		};
	}
}

#pragma warning(pop)