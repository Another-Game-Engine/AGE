#include "LastRoom.hh"
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

	LastRoom::LastRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
	}

	LastRoom::~LastRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool LastRoom::init()
	{
		// load
		auto s = _scene.lock();
		hotZoneLastEngine = createHotZone("Last->Engine", "HZ-engine-last");
		return true;
	}

	bool LastRoom::_enable()
	{
		auto scene = _scene.lock();
		return true;
	}

	bool LastRoom::_disable()
	{
		auto scene = _scene.lock();
		return true;
	}