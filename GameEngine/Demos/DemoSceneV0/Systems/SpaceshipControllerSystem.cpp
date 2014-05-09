#include <Systems/SpaceshipControllerSystem.hh>
#include <Components/SpaceshipController.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/RigidBody.hpp>
#include <Components/MeshRenderer.hh>

#include <Physic/Utils/BtConversion.hpp>

#include <Core/Input.hh>
#include <Core/Timer.hh>

#include <SDL/SDL.h>
#include <glm/glm.hpp>

#include "MyTags.hpp"

SpaceshipControllerSystem::SpaceshipControllerSystem(std::weak_ptr<AScene> &&scene)
: System(std::move(scene))
, _spaceshipsFilter(std::move(scene))
{
}

SpaceshipControllerSystem::~SpaceshipControllerSystem()
{
}

bool SpaceshipControllerSystem::initialize()
{
	_spaceshipsFilter.requireComponent<Component::SpaceshipController>();
	_spaceshipsFilter.requireComponent<Component::CameraComponent>();
	_timer = 0.0f;
	return true;
}

void SpaceshipControllerSystem::mainUpdate(double time)
{
	auto scene = _scene.lock();
	for (auto e : _spaceshipsFilter.getCollection())
	{
		auto camera = scene->getComponent<Component::CameraComponent>(e);
		auto control = scene->getComponent<Component::SpaceshipController>(e);
		auto rigidBody = scene->getComponent<Component::RigidBody>(control->spaceShip);

		glm::mat4	&transform = scene->getLocalTransform(control->spaceShip);

		if (scene->getInstance<Input>()->getKey(SDLK_w))
		{
			transform = glm::rotate(transform, 1.0f * (float)time * control->angularSpeed, glm::vec3(1, 0, 0));
			rigidBody->getBody().setWorldTransform(convertGLMTransformToBullet(transform));
		}
		if (scene->getInstance<Input>()->getKey(SDLK_s))
		{
			transform = glm::rotate(transform, -1.0f * (float)time * control->angularSpeed, glm::vec3(1, 0, 0));
			rigidBody->getBody().setWorldTransform(convertGLMTransformToBullet(transform));
		}
		if (scene->getInstance<Input>()->getKey(SDLK_a))
		{
			transform = glm::rotate(transform, -1.0f * (float)time * control->angularSpeed, glm::vec3(0, 0, 1));
			rigidBody->getBody().setWorldTransform(convertGLMTransformToBullet(transform));
		}
		if (scene->getInstance<Input>()->getKey(SDLK_d))
		{
			transform = glm::rotate(transform, 1.0f * (float)time * control->angularSpeed, glm::vec3(0, 0, 1));
			rigidBody->getBody().setWorldTransform(convertGLMTransformToBullet(transform));
		}
		if (scene->getInstance<Input>()->getKey(SDLK_SPACE))
		{
			glm::vec4  velocity = (transform * glm::vec4(0, 0, 1, 0)) * control->acceleration * (float)time;
//			rigidBody->getBody().setWorldTransform(convertGLMTransformToBullet(glm::translate(transform, glm::vec3(velocity))));
			rigidBody->getBody().applyCentralImpulse(convertGLMVectorToBullet(glm::vec3(velocity)));
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

		if (scene->getInstance<Input>()->getInput(SDL_BUTTON_LEFT) && _timer > 0.5f)
		{
			Entity bullet = scene->createEntity();
			glm::vec3 impulse = glm::vec3(0, 0, 1);

			impulse = glm::vec3(transform * glm::vec4(impulse, 0));

			impulse *= 50.0f;

			transform[3] = glm::vec4(glm::vec3(transform * glm::vec4(0, 0, 3, 1)), 1);
			scene->getLocalTransform(bullet) = transform;
			auto mesh = scene->addComponent<Component::MeshRenderer>(bullet, _scene.lock()->getInstance<AssetsManager>()->get<ObjFile>("obj__ball"));
			mesh->setShader("MaterialBasic");
			auto body = scene->addComponent<Component::RigidBody>(bullet, _scene, 0.5f);
			body->setCollisionShape(bullet, Component::RigidBody::SPHERE);
			body->getBody().setRestitution(0.0f);
			body->getBody().setActivationState(DISABLE_DEACTIVATION);
			body->getBody().setDamping(0.0f, 0.0f);
			body->getBody().applyCentralImpulse(convertGLMVectorToBullet(impulse));
			scene->addTag(bullet, MyTags::BULLET_TAG);

			auto lightComponent = scene->addComponent<Component::PointLight>(bullet);
			lightComponent->lightData.colorRange = glm::vec4(1, 0, 0, 50.0f);
			lightComponent->lightData.positionPower.w = 10.0f;
			lightComponent->lightData.hasShadow = -1;

			_timer = 0.0f;
			_bullets.push(bullet);
			if (_bullets.size() > 10)
			{
				_scene.lock()->destroy(_bullets.front());
				_bullets.pop();
			}
		}
	}
	_timer += static_cast<float>(_scene.lock()->getInstance<Timer>()->getElapsed());
}
