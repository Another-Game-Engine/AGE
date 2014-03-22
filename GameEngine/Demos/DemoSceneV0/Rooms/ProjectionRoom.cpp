#include "ProjectionRoom.hh"
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

	ProjectionRoom::ProjectionRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
	}

	ProjectionRoom::~ProjectionRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool ProjectionRoom::init()
	{
		// load
		auto s = _scene.lock();
		hotZoneEntranceProjection = createHotZone("Projection->Entrance", "HZ-entrance-projection");
		hotZoneProjectionPhysics = createHotZone("Projection->Physics", "HZ-projection-physics");
		return true;
	}

	bool ProjectionRoom::_enable()
	{
		auto scene = _scene.lock();
		return true;
	}

	bool ProjectionRoom::_disable()
	{
		auto scene = _scene.lock();
		return true;
	}