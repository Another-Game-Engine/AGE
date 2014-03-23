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
#include <Components/SpotLight.hh>
#include <Components/EntityPlacable.hpp>
#include <Components/TransformationRegister.hpp>

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
		hotZoneProjectionCircle = createHotZone("Projection->Circle", "HZ-projection-circle");
		return true;
	}

	bool ProjectionRoom::_enable()
	{
		auto scene = _scene.lock();
		{
			spotLight = scene->createEntity();
			//light->addComponent<Component::MeshRenderer>(meshObj)->setShader("MaterialBasic");
			auto l = spotLight->addComponent<Component::SpotLight>();
			l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 100.0f);
			l->lightData.positionPower.w = 10.0f;
			l->projection = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
			l->lightData.shadowId = 1;
			spotLight->addComponent<Component::EntityPlacable>("projector-spotlight");
			spotLight->addComponent<Component::TransformationRegister>("projector-spotlight");
		}
		for (auto i = 0; i < 6; ++i)
		{
			auto light = scene->createEntity();
			auto l = light->addComponent<Component::PointLight>();
			l->lightData.colorRange = glm::vec4(1.0f, 0.905f, 0.5764f, 20.0f); // distance
			l->lightData.positionPower.w = 0.3f; // intensite
			light->addComponent<Component::TransformationRegister>("projection-room-pointlight-" + std::to_string(i));
			light->addComponent<Component::EntityPlacable>("projection-room-pointlight-" + std::to_string(i));
			pointLights.push_back(light);
		}
		{
			auto light = scene->createEntity();
			auto l = light->addComponent<Component::PointLight>();
			l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 20.0f); // distance
			l->lightData.positionPower.w = 0.9f; // intensite
			light->addComponent<Component::TransformationRegister>("projection-room-pointlight-6");
			light->addComponent<Component::EntityPlacable>("projection-room-pointlight-6");
			pointLights.push_back(light);
		}
		{
			boringText = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__boring-room");
			if (!meshObj)
				return false;
			auto meshComponent = boringText->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			boringText->addComponent<Component::TransformationRegister>("boring-text");
			boringText->addComponent<Component::EntityPlacable>("boring-text");
		}
		return true;
	}

	bool ProjectionRoom::_disable()
	{
		auto scene = _scene.lock();
		scene->destroy(spotLight);
		for (auto e : pointLights)
			scene->destroy(e);
		pointLights.clear();
		scene->destroy(boringText);
		return true;
	}