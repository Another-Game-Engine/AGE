#pragma once

#include <Components/Component.hh>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Entities/Entity.hh>

namespace Component
{
	class PointLight;
}

namespace Component
{
	struct SpaceshipController : public Component::ComponentBase<SpaceshipController>
	{
		SpaceshipController();
		SpaceshipController(SpaceshipController const &o)
		{
			dist = o.dist;
			camSpeed = o.camSpeed;
			angularSpeed = o.angularSpeed;
			acceleration = o.acceleration;
			spaceShip = o.spaceShip;
			currentVelocity = o.currentVelocity;
			currentLookAt = o.currentLookAt;
			currentPosition = o.currentPosition;
			light = o.light;
		}

		SpaceshipController &operator=(SpaceshipController const &o)
		{
			dist = o.dist;
			camSpeed = o.camSpeed;
			angularSpeed = o.angularSpeed;
			acceleration = o.acceleration;
			spaceShip = o.spaceShip;
			currentVelocity = o.currentVelocity;
			currentLookAt = o.currentLookAt;
			currentPosition = o.currentPosition;
			light = o.light;
			return *this;
		}

		virtual ~SpaceshipController(void);
		void init(const Entity &e, float dist);
		virtual void reset();

		//////
		////
		// Serialization

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(CEREAL_NVP(dist));
		}

		// !Serialization
		////
		//////

		float					dist;
		float					camSpeed;
		float					angularSpeed;
		float					acceleration;
		Entity                  spaceShip;

		glm::vec3				currentVelocity;

		glm::vec3				currentLookAt;
		glm::vec3				currentPosition;

		std::shared_ptr<Component::PointLight>	light;
	private:
	};
}