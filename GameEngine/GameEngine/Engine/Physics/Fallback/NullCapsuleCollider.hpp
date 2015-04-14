#pragma once

#include <Physics/CapsuleColliderInterface.hpp>
#include <Physics/Fallback/NullCollider.hpp>

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class NullCapsuleCollider final : public CapsuleColliderInterface, public NullCollider
		{
			// Friendships
			friend ObjectPool < NullCapsuleCollider >;

		public:
			// Constructors
			NullCapsuleCollider(void) = delete;

			NullCapsuleCollider(WorldInterface *world, Private::GenericData *data);

			NullCapsuleCollider(const NullCapsuleCollider &) = delete;

			// Assignment Operators
			NullCapsuleCollider &operator=(const NullCapsuleCollider &) = delete;

		private:
			// Destructor
			~NullCapsuleCollider(void) = default;
			
			// Inherited Methods
			void setCenter(const glm::vec3 &center) override final;

			glm::vec3 getCenter(void) const override final;

			void setHeight(float height) override final;

			float getHeight(void) const override final;

			void setRadius(float radius) override final;

			float getRadius(void) const override final;
		};
	}
}

#pragma warning(pop)