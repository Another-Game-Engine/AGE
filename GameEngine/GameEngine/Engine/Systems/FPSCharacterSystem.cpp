#include "FPSCharacterSystem.hh"

#include <Components/RigidBody.hpp>
#include <Components/Collider.hpp>
#include <Components/CameraComponent.hpp>
#include <Components/CharacterController.hh>

#include <Entity/Entity.hh>
#include <Entity/EntityData.hh>

#include <Core/AScene.hh>
#include <Systems/PhysicsSystem.hpp>

#include <Physics/RaycasterInterface.hpp>
#include <Physics/RaycastHit.hpp>

#include <Physics/CharacterControllerInterface.hh>
#include <Core/Inputs/Input.hh>

namespace AGE
{
	FPSCharacterSystem::FPSCharacterSystem(AScene *scene) :
		System(scene),
		_filter(scene)
	{
	}

	bool FPSCharacterSystem::initialize()
	{
		_filter.requireComponent<CharacterController>();
		return (true);
	}

	void FPSCharacterSystem::updateBegin(float time)
	{
	}

	void FPSCharacterSystem::mainUpdate(float time)
	{

	}

	void FPSCharacterSystem::updateEnd(float time)
	{
	}
}
