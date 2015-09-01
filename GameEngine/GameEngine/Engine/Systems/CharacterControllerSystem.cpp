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

namespace AGE
{
	CharacterControllerSystem::CharacterControllerSystem(AScene *scene) :
		System(scene),
		_filter(scene)
	{
	}

	bool CharacterControllerSystem::initialize()
	{
		_filter.requireComponent<RigidBody>();
		_filter.requireComponent<Collider>();
		_filter.requireComponent<CharacterController>();
		return (true);
	}

	void CharacterControllerSystem::updateBegin(float time)
	{
		for (Entity const &entity : _filter.getCollection())
		{
			entity->getComponent<Collider>()->setFilterGroup(Physics::FilterGroup::GroupII);
		}
	}

	void CharacterControllerSystem::mainUpdate(float time)
	{
		Physics::RaycasterInterface *rc = _scene->getSystem<PhysicsSystem>()->getPhysics()->getWorld()->getRaycaster();
		for (Entity const &entity : _filter.getCollection())
		{
			glm::mat4 transform = entity->getLink().getGlobalTransform();
			bool result = rc->raycast(glm::vec3(transform[3]) + glm::vec3(0, -0.5, 0), glm::vec3(0, -1, 0), 0.3f, Physics::All);
			if (result)
			{
				if (entity->getComponent<RigidBody>()->isAffectedByGravity() == true)
					entity->getComponent<RigidBody>()->affectByGravity(false);
				if (entity->getComponent<RigidBody>()->isKinematic() == false)
					entity->getComponent<RigidBody>()->setAsKinematic(true);
			}
			else
			{
				if (entity->getComponent<RigidBody>()->isAffectedByGravity() == false)
					entity->getComponent<RigidBody>()->affectByGravity(true);
				if (entity->getComponent<RigidBody>()->isKinematic() == true)
					entity->getComponent<RigidBody>()->setAsKinematic(false);
			}
		}
	}

	void CharacterControllerSystem::updateEnd(float time)
	{

	}
}
