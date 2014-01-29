#ifndef   __SPACESHIP_CONT_SYSTEM_HPP__
# define  __SPACESHIP_CONT_SYSTEM_HPP__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Systems/System.h>
#include <Components/SpaceshipController.hpp>
#include <Entities/EntityData.hh>
#include <Core/Engine.hh>
#include <Components/Collision.hpp>
#include <Context/SdlContext.hh>


class SpaceshipControllerSystem : public System
{
public:
	SpaceshipControllerSystem(AScene *scene)
		: System(scene)
		, _filter(scene)
	{}
	virtual ~SpaceshipControllerSystem(){}
private:

	EntityFilter _filter;

	virtual void updateBegin(double time)
	{
	}

	virtual void updateEnd(double time)
	{}

	virtual void mainUpdate(double time)
	{
		for (auto e : _filter.getCollection())
		{
			auto c = e->getComponent<Component::SpaceshipController>();
			if (e->hasComponent<Component::Collision>())
			{
				auto lol = e->getComponent<Component::Collision>();
				std::cout << "lol" << std::endl;
			}
			updateComponent(e, c, time);
		}
	}

	void updateComponent(Entity &entity, SmartPointer<Component::SpaceshipController> c, double time)
	{
			c->resetControls();
			auto &inputs = _scene->getEngine().getInstance<Input>();
			auto &controls = c->controls;
			auto &keys = c->keys;

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
			static std::vector<Entity> balls;

			if (controls[Component::SpaceshipController::SHOOT])
			{
				Entity b = _scene->createEntity();
				b->setLocalTransform() = entity->getLocalTransform();
				b->addComponent<Component::GraphNode>();
				auto rigidBody = b->addComponent<Component::RigidBody>();
				rigidBody->setMass(1.0f);
				rigidBody->setCollisionShape(Component::RigidBody::SPHERE);
//				rigidBody->getBody().applyCentralImpulse(btVector3(0, 0, 1000));
				auto mesh = b->addComponent<Component::MeshRenderer>(AMediaFile::get<ObjFile>("obj__ball"));
				mesh->setShader("MaterialBasic");
//				_scene->destroy(b);
				balls.push_back(b);
			}
			if (inputs.getKey(SDLK_p))
			{
				for (auto e : balls)
					_scene->destroy(e);
				balls.clear();
			}
			entity->setLocalTransform() = glm::translate(entity->getLocalTransform(), direction);
	}

	virtual void initialize()
	{
		_filter.require<Component::SpaceshipController>();
		_filter.require<Component::RigidBody>();
		SDL_SetRelativeMouseMode(SDL_bool(true));
	}
};

#endif    //__KINE_CHAR_CONT_SYSTEM_HPP__