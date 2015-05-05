#pragma once

#include <Core/AScene.hh>
#include <Physics/Collision.hpp>

namespace AGE
{
	namespace Private
	{
		class CollisionSystem;
	}

	namespace Physics
	{
		class ICollisionListener
		{
			// Friendships
			friend ::AGE::Private::CollisionSystem;

		public:
			// Constructors
			ICollisionListener(void) = delete;

			ICollisionListener(AScene *scene);

			ICollisionListener(const ICollisionListener &) = delete;

			// Assignment Operators
			ICollisionListener &operator=(const ICollisionListener &) = delete;

			// Destructor
			virtual ~ICollisionListener(void);

		protected:
			// Virtual Methods
			virtual void onCollisionEnter(Entity currentEntity, Entity hitEntity, const std::vector<Physics::Contact> &contacts);

			virtual void onCollisionStay(Entity currentEntity, Entity hitEntity, const std::vector<Physics::Contact> &contacts);

			virtual void onCollisionExit(Entity currentEntity, Entity hitEntity, const std::vector<Physics::Contact> &contacts);

		private:
			// Attributes
			AScene *scene = nullptr;
		};
	}
}