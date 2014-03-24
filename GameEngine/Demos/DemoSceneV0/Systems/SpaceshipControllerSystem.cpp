#include <Systems/SpaceshipControllerSystem.hh>
#include <Components/SpaceshipController.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/RigidBody.hpp>

#include <Physic/Utils/BtConversion.hpp>

#include <SDL/SDL.h>
#include <glm/glm.hpp>

SpaceshipControllerSystem::SpaceshipControllerSystem(std::weak_ptr<AScene> scene) :
								System(scene),
								_spaceshipsFilter(scene)
{
}

SpaceshipControllerSystem::~SpaceshipControllerSystem()
{
}

bool SpaceshipControllerSystem::initialize()
{
	_spaceshipsFilter.requireComponent<Component::SpaceshipController>();
	_spaceshipsFilter.requireComponent<Component::CameraComponent>();
	return true;
}

void SpaceshipControllerSystem::mainUpdate(double time)
{
	for (auto e : _spaceshipsFilter.getCollection())
	{
		auto camera = e->getComponent<Component::CameraComponent>();
		auto control = e->getComponent<Component::SpaceshipController>();
		auto rigidBody = control->spaceShip->getComponent<Component::RigidBody>();

		glm::mat4	transform = control->spaceShip->getLocalTransform();

		if (_scene.lock()->getInstance<Input>()->getKey(SDLK_w))
			rigidBody->getBody().applyTorqueImpulse(convertGLMVectorToBullet(glm::vec3(transform * glm::vec4(0.1 * time * control->angularSpeed, 0, 0, 0))));
		if (_scene.lock()->getInstance<Input>()->getKey(SDLK_s))
			rigidBody->getBody().applyTorqueImpulse(convertGLMVectorToBullet(glm::vec3(transform * glm::vec4(-0.1 * time * control->angularSpeed, 0, 0, 0))));
		if (_scene.lock()->getInstance<Input>()->getKey(SDLK_a))
			rigidBody->getBody().applyTorqueImpulse(convertGLMVectorToBullet(glm::vec3(transform * glm::vec4(0, 0, -0.1 * time * control->angularSpeed, 0))));
		if (_scene.lock()->getInstance<Input>()->getKey(SDLK_d))
			rigidBody->getBody().applyTorqueImpulse(convertGLMVectorToBullet(glm::vec3(transform * glm::vec4(0, 0, 0.1 * time * control->angularSpeed, 0))));
		if (_scene.lock()->getInstance<Input>()->getKey(SDLK_SPACE))
		{
			glm::vec4  velocity = (transform * glm::vec4(0, 0, 1, 0)) * control->acceleration * (float)time;
			rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(glm::vec3(velocity * 100.0f)));
			control->light->lightData.positionPower.w += (float)time * 10.0f;
			control->light->lightData.positionPower.w = glm::clamp(control->light->lightData.positionPower.w, 0.0f, 50.0f);
		}
		else
		{
			control->light->lightData.positionPower.w -= (float)time * 10.0f;
			control->light->lightData.positionPower.w = glm::clamp(control->light->lightData.positionPower.w, 0.0f, 50.0f);
		}

		glm::vec3		posDif = glm::vec3(transform * glm::vec4(0, 1, -control->dist, 1)) - control->currentPosition;

		control->currentPosition = (control->currentPosition + posDif * (float)time * control->camSpeed);
		control->currentLookAt = glm::vec3(transform[3]);

		camera->lookAtTransform = glm::lookAt(control->currentPosition, control->currentLookAt, glm::vec3(transform * glm::vec4(0, 1, 0, 0)));
	}
}
