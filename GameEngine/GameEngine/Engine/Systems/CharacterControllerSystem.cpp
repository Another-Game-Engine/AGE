#include "CharacterControllerSystem.hh"

#include <Components/RigidBody.hpp>
#include <Components/Collider.hpp>
#include <Components/CameraComponent.hpp>

namespace AGE
{
	CharacterControllerSystem::CharacterControllerSystem(AScene *scene)
	{
	}

	bool CharacterControllerSystem::initialize()
	{
//		_cameras.requireComponent<CameraComponent>();
//		_cameras.requireComponent<RigidBody>();
//		_cameras.requireComponent<Collider>();
		return (true);
	}

	void CharacterControllerSystem::updateBegin(float time)
	{

	}

	void CharacterControllerSystem::mainUpdate(float time)
	{

	}

	void CharacterControllerSystem::updateEnd(float time)
	{

	}

}
