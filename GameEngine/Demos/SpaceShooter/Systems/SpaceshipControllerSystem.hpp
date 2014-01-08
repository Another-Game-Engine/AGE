#ifndef   __SPACESHIP_CONT_SYSTEM_HPP__
# define  __SPACESHIP_CONT_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Systems/System.h>
#include <Components/SpaceshipController.hpp>
#include <Entities/Entity.hh>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include <Context/SdlContext.hh>


class SpaceshipControllerSystem : public System
{
public:
	SpaceshipControllerSystem(AScene *scene) : System(scene)
	{}
	virtual ~SpaceshipControllerSystem(){}
private:
	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _collection)
		{
			auto c = e->getComponent<Component::SpaceshipController>();

			updateComponent(e, c, time);
		}
	}

	void updateComponent(Handle &entity, SmartPointer<Component::SpaceshipController> c, double time)
	{
			c->resetControls();
			auto &inputs = _scene->getEngine().getInstance<Input>();
			auto &controls = c->getControls();
			auto &keys = c->getKeys();

			float yAngle = inputs.getMouseDelta().y * 0.3f;
			float xAngle = - inputs.getMouseDelta().x * 0.3f;

			entity->setLocalTransform() = glm::rotate(entity->getLocalTransform(), yAngle, glm::vec3(1, 0, 0));
			entity->setLocalTransform() = glm::rotate(entity->getLocalTransform(), xAngle, glm::vec3(0, 1, 0));

			// UPDATE KEYS
			for (unsigned int i = 0; i < controls.size(); ++i)
			{
				controls[i] = inputs.getKey(keys[i]);
			}

			auto forwardDir = glm::vec3(0,0,1);
			auto upDir = glm::vec3(0,1,0);
			auto sideDir = glm::vec3(1,0,0);


			glm::vec3 direction = glm::vec3(0);
			if (controls[Component::SpaceshipController::LEFT])
				direction += sideDir * (float)(10.0f * time);

			if (controls[Component::SpaceshipController::RIGHT])
				direction -= sideDir * (float)(10.0f * time);

			if (controls[Component::SpaceshipController::FORWARD])
				direction += forwardDir * (float)(10.0f * time);

			if (controls[Component::SpaceshipController::BACKWARD])
				direction -= forwardDir * (float)(10.0f * time);
			entity->setLocalTransform() = glm::translate(entity->getLocalTransform(), direction);
	}

	virtual void initialize()
	{
		require<Component::SpaceshipController>();
		SDL_SetRelativeMouseMode(SDL_bool(true));
	}
};

#endif    //__KINE_CHAR_CONT_SYSTEM_HPP__