#pragma once

#include "FilterGroup.hpp"
#include <glm/glm.hpp>

namespace AGE
{
	namespace Physics
	{
		class WorldInterface;

		enum CollisionFlags
		{
			NO_COLLISION = 0,
			COLLISION_SIDES = (1 << 0),
			COLLISION_UP = (1 << 1),
			COLLISION_DOWN = (1 << 2)
		};

		class CharacterControllerInterface
		{
		public:
			CharacterControllerInterface(void) = delete;
			CharacterControllerInterface(WorldInterface *world);
			virtual ~CharacterControllerInterface(void) = default;

			virtual uint32_t move(glm::vec3 const &displacement, float minDist, float elapsedTime, FilterGroup group) = 0;
			virtual void setRadius(float radius) = 0;
			virtual void setHeight(float height) = 0;
			virtual void resize(float height) = 0;

			virtual float getRadius() const = 0;
			virtual float getHeight() const = 0;

			virtual glm::vec3 getPosition() const = 0;
			virtual bool setPosition(glm::vec3 const &position) = 0;

			WorldInterface *_world;
		};
	}
}

#include "CharacterControllerInterface.inl"