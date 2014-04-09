#pragma once

#include "Component.hh"
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <Components/PointLight.hh>

namespace Component
{
	struct SpaceshipController : public Component::ComponentBase<SpaceshipController>
	{
		SpaceshipController();
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
		SpaceshipController(SpaceshipController const &);
		SpaceshipController &operator=(SpaceshipController const &);
	};
}