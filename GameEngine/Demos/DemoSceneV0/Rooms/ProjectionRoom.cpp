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
#include <Systems/PistolSystem.hpp>

	ProjectionRoom::ProjectionRoom(std::weak_ptr<AScene> &&scene)
		: Room(std::move(scene))
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
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/Slides.CPDAnimation"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/Powerpoint.CPDAnimation"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/PowerpointTroll.CPDAnimation"));
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__office_chair.cpd"));

		return true;
	}

	bool ProjectionRoom::_enable()
	{
		auto scene = _scene.lock();
		scene->activateSystem<PistolSystem>();

		//@CESAR TODO
		//{
		//	spotLight = scene->createEntity();
		//	//light->addComponent<Component::MeshRenderer>(meshObj)->setShader("MaterialBasic");
		//	auto l = spotLight->addComponent<Component::SpotLight>();
		//	l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 100.0f);
		//	l->lightData.positionPower.w = 10.0f;
		//	l->projection = glm::perspective(40.0f, 1.0f, 0.1f, 100.0f);
		//	l->lightData.shadowId = 1;
		//	spotLight->addComponent<Component::EntityPlacable>("projector-spotlight");
		//	spotLight->addComponent<Component::TransformationRegister>("projector-spotlight");
		//}
		//for (auto i = 0; i < 6; ++i)
		//{
		//	auto light = scene->createEntity();
		//	auto l = light->addComponent<Component::PointLight>();
		//	l->lightData.colorRange = glm::vec4(1.0f, 0.905f, 0.5764f, 20.0f); // distance
		//	l->lightData.positionPower.w = 0.3f; // intensite
		//	light->addComponent<Component::TransformationRegister>("projection-room-pointlight-" + std::to_string(i));
		//	light->addComponent<Component::EntityPlacable>("projection-room-pointlight-" + std::to_string(i));
		//	pointLights.push_back(light);
		//}
		//{
		//	auto light = scene->createEntity();
		//	auto l = light->addComponent<Component::PointLight>();
		//	l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 20.0f); // distance
		//	l->lightData.positionPower.w = 0.9f; // intensite
		//	light->addComponent<Component::TransformationRegister>("projection-room-pointlight-6");
		//	light->addComponent<Component::EntityPlacable>("projection-room-pointlight-6");
		//	pointLights.push_back(light);
		//}
		//{
		//	boringText = scene->createEntity();
		//	auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__boring-room");
		//	if (!meshObj)
		//		return false;
		//	auto meshComponent = boringText->addComponent<Component::MeshRenderer>(meshObj);
		//	meshComponent->setShader("MaterialBasic");
		//	boringText->addComponent<Component::TransformationRegister>("boring-text");
		//	boringText->addComponent<Component::EntityPlacable>("boring-text");
		//}
		//{
		//	slides = scene->createEntity();
		//	auto sprite = slides->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Slides", "slide1"));
		//	sprite->delay = 1.0f / 10.0f;
		//	slides->addComponent<Component::TransformationRegister>("slides");
		//	slides->addComponent<Component::EntityPlacable>("slides");
		//	slides->addTag(MyTags::SLIDE_TAG);
		//}
		//{
		//	powerpoint = scene->createEntity();
		//	auto sprite = powerpoint->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Powerpoint", "frame"));
		//	sprite->delay = 1.0f / 10.0f;
		//	powerpoint->addComponent<Component::TransformationRegister>("powerpoint-gif");
		//	powerpoint->addComponent<Component::EntityPlacable>("powerpoint-gif");
		//}
		//{
		//	auto e = scene->createEntity();
		//	auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("PowerpointTroll", "troll5/frame"));
		//	sprite->delay = 1.0f / 10.0f;
		//	e->addComponent<Component::TransformationRegister>("powerpoint-troll-1");
		//	e->addComponent<Component::EntityPlacable>("powerpoint-troll-1");
		//	trolls.push_back(e);
		//}
		//{
		//	auto e = scene->createEntity();
		//	auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("PowerpointTroll", "troll4/frame"));
		//	sprite->delay = 1.0f / 10.0f;
		//	e->addComponent<Component::TransformationRegister>("powerpoint-troll-2");
		//	e->addComponent<Component::EntityPlacable>("powerpoint-troll-2");
		//	trolls.push_back(e);
		//}
		//{
		//	auto e = scene->createEntity();
		//	auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("PowerpointTroll", "troll3"));
		//	sprite->delay = 1.0f / 10.0f;
		//	e->addComponent<Component::TransformationRegister>("powerpoint-troll-3");
		//	e->addComponent<Component::EntityPlacable>("powerpoint-troll-3");
		//	trolls.push_back(e);
		//}
		//{
		//	auto e = scene->createEntity();
		//	auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("PowerpointTroll", "troll2"));
		//	sprite->delay = 1.0f / 10.0f;
		//	e->addComponent<Component::TransformationRegister>("powerpoint-troll-4");
		//	e->addComponent<Component::EntityPlacable>("powerpoint-troll-4");
		//	trolls.push_back(e);
		//}
		//{
		//	auto e = scene->createEntity();
		//	auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("PowerpointTroll", "troll1"));
		//	sprite->delay = 1.0f / 10.0f;
		//	e->addComponent<Component::TransformationRegister>("powerpoint-troll-5");
		//	e->addComponent<Component::EntityPlacable>("powerpoint-troll-5");
		//	trolls.push_back(e);
		//}
		//{
		//	for (auto i = 9; i < 10; ++i)
		//	{
		//		auto e = scene->createEntity();
		//		e->setLocalTransform(glm::scale(e->getLocalTransform(), glm::vec3(0.6f)));
		//		auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__office_chair");
		//		if (!meshObj)
		//			return false;
		//		auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
		//		meshComponent->setShader("MaterialBasic");
		//		auto rigidBody = e->addComponent<Component::RigidBody>(0.0f);
		//		rigidBody->setMass(0.0f);
		//		rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_dynamic_office_chair");
		//		e->addComponent<Component::TransformationRegister>("office-chair-" + std::to_string(i));
		//		e->addComponent<Component::EntityPlacable>("office-chair-" + std::to_string(i));
		//		trolls.push_back(e);
		//	}
		//}
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
		scene->destroy(slides);
		scene->deactivateSystem<PistolSystem>();
		scene->destroy(powerpoint);
		for (auto e : trolls)
			scene->destroy(e);
		trolls.clear();
		return true;
	}