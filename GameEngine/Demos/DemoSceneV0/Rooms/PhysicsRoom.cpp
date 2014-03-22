#include "PhysicsRoom.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>
#include <Sprite/SpriteManager.hh>
#include <Components/SpriteComponent.hh>
#include <Systems/EntityPlacingSystem.hpp>
#include <Systems/TransformationRegisterSystem.hpp>
#include <Systems/HotZoneSystem.hpp>
#include <Components/CollisionBody.hpp>

	PhysicsRoom::PhysicsRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
	}

	PhysicsRoom::~PhysicsRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool PhysicsRoom::init()
	{
		// load
		auto s = _scene.lock();
		hotZonePhysicsProjection = createHotZone("Physics->Projection", "HZ-projection-physics");
		return true;
	}

	bool PhysicsRoom::_enable()
	{
		auto scene = _scene.lock();
		return true;
	}

	bool PhysicsRoom::_disable()
	{
		auto scene = _scene.lock();
		return true;
	}