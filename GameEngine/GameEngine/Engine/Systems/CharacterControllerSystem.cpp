#include "CharacterControllerSystem.hh"

#include <Components/RigidBody.hpp>
#include <Components/Collider.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/CharacterController.hh>

#include <Entities/Entity.hh>
#include <Entities/EntityData.hh>

#include <Core/AScene.hh>
#include <Systems/PhysicsSystem.hpp>

#include <Physics/RaycasterInterface.hpp>
#include <Physics/RaycastHit.hpp>

#include <Core/Inputs/Input.hh>

namespace AGE
{
	CharacterControllerSystem::CharacterControllerSystem(AScene *scene, bool isEditor) :
		System(scene),
		_filter(scene),
		_isEditor(isEditor)
	{
	}

	bool CharacterControllerSystem::initialize()
	{
		_filter.requireComponent<CharacterController>();
		return (true);
	}

	void CharacterControllerSystem::updateBegin(float time)
	{
		for (Entity const &entity : _filter.getCollection())
		{
			glm::vec3 pos = entity.getLinkPtr()->getPosition();
			entity->getComponent<CharacterController>()->controller->setPosition(pos);
		}
	}

	void CharacterControllerSystem::mainUpdate(float time)
	{
		float camTranslationSpeed = 5.0f;
		float maxAcceleration = 10.0f;
		float camRotationSpeed = 2.0f;
		glm::vec3 relativeMove(0);
		glm::vec2 deltaCamAngles(0);
		Input *inputs = _scene->getInstance<Input>();

		// translations
		if (inputs->getPhysicalKeyPressed(AGE_w))
			relativeMove += glm::vec3(0, 0, -1);
		if (inputs->getPhysicalKeyPressed(AGE_s))
			relativeMove += glm::vec3(0, 0, 1);
		if (inputs->getPhysicalKeyPressed(AGE_a))
			relativeMove += glm::vec3(-1, 0, 0);
		if (inputs->getPhysicalKeyPressed(AGE_d))
			relativeMove += glm::vec3(1, 0, 0);
		if (relativeMove != glm::vec3(0))
			relativeMove = glm::normalize(relativeMove) * camTranslationSpeed * time;
		// rotations
		if (inputs->getPhysicalKeyPressed(AGE_UP))
			deltaCamAngles.x += camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(AGE_DOWN))
			deltaCamAngles.x -= camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(AGE_RIGHT))
			deltaCamAngles.y -= camRotationSpeed * time;
		if (inputs->getPhysicalKeyPressed(AGE_LEFT))
			deltaCamAngles.y += camRotationSpeed * time;
		for (Entity const &entity : _filter.getCollection())
		{
			CharacterController *component = entity->getComponent<CharacterController>();

			component->camAngles += deltaCamAngles;
			glm::quat finalOrientation = glm::quat(glm::vec3(component->camAngles, 0));
			entity.getLinkPtr()->setOrientation(finalOrientation);

			glm::vec3 displacement = finalOrientation * relativeMove;

			if (_isEditor)
			{
				if (component->controller)
				{
					component->collisionFlags = component->controller->move(displacement, 0.001f, time, Physics::FilterGroup::All);
				}
			}
			else
			{
				if ((component->collisionFlags & Physics::COLLISION_DOWN) != 0)
					component->velocity.y = 0.0f;
				else if (component->velocity.y > -200.0f)
					component->velocity.y -= 0.5f * time;
				displacement += component->velocity;
				if (component->controller)
				{
					component->collisionFlags = component->controller->move(displacement, 0.001f, time, Physics::FilterGroup::All);
				}
			}
		}
	}

	void CharacterControllerSystem::updateEnd(float time)
	{
		for (Entity const &entity : _filter.getCollection())
		{
			glm::vec3 pos = entity->getComponent<CharacterController>()->controller->getPosition();
			entity.getLinkPtr()->setPosition(pos);
		}
	}
}
