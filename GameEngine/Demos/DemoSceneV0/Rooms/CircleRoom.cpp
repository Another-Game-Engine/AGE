#include "CircleRoom.hh"
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

	CircleRoom::CircleRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
	}

	CircleRoom::~CircleRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool CircleRoom::init()
	{
		// load
		auto s = _scene.lock();

		return true;
	}

	bool CircleRoom::_enable()
	{
		auto scene = _scene.lock();
		return true;
	}

	bool CircleRoom::_disable()
	{
		auto scene = _scene.lock();
		return true;
	}