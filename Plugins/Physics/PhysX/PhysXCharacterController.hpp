#pragma once

#include <CharacterControllerInterface.hh>
#include "characterkinematic/PxControllerManager.h"

#pragma warning(push)
#pragma warning(disable: 4250)

namespace AGE
{
	namespace Physics
	{
		class PhysXCharacterController : public CharacterControllerInterface
		{
		public:
			PhysXCharacterController(void) = delete;
			PhysXCharacterController(WorldInterface *world);
			virtual ~PhysXCharacterController(void);

			virtual uint32_t move(glm::vec3 const &displacement, float minDist, float elapsedTime, FilterGroup group);
			virtual void setRadius(float radius);
			virtual void setHeight(float height);
			virtual void resize(float height);

			virtual float getRadius() const;
			virtual float getHeight() const;

			virtual glm::vec3 getPosition() const;
			virtual bool setPosition(glm::vec3 const &position);

		protected:

			static physx::PxControllerManager *_manager;
			physx::PxController *_controller;
		};
	}
}

#pragma warning(pop)