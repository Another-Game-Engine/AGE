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
#include <Components/SpotLight.hh>

	LastRoom::LastRoom(std::weak_ptr<AScene> &&scene)
		: Room(std::move(scene))
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
		//@CESAR TODO

		//auto scene = _scene.lock();
		//{
		//	auto spotLight = scene->createEntity();
		//	auto l = spotLight->addComponent<Component::SpotLight>();
		//	l->lightData.colorRange = glm::vec4(0.909f, 0.2f, 0.447f, 50.0f);
		//	l->lightData.positionPower.w = 1.0f;
		//	l->projection = glm::perspective(120.0f, 1.0f, 0.1f, 100.0f);
		//	l->lightData.shadowId = 1;
		//	spotLight->addComponent<Component::EntityPlacable>("last-room-spotlight-1");
		//	spotLight->addComponent<Component::TransformationRegister>("last-room-spotlight-1");
		//	col.push_back(spotLight);
		//}
		//{
		//	auto spotLight = scene->createEntity();
		//	auto l = spotLight->addComponent<Component::SpotLight>();
		//	l->lightData.colorRange = glm::vec4(1,1,1, 50);
		//	l->lightData.positionPower.w = 1.0f;
		//	l->projection = glm::perspective(120.0f, 1.0f, 0.1f, 100.0f);
		//	l->lightData.shadowId = 1;
		//	spotLight->addComponent<Component::EntityPlacable>("last-room-spotlight-2");
		//	spotLight->addComponent<Component::TransformationRegister>("last-room-spotlight-2");
		//	col.push_back(spotLight);
		//}
		//{
		//	for (auto i = 0; i < 3; ++i)
		//	{
		//		auto light = scene->createEntity();
		//		auto l = light->addComponent<Component::PointLight>();
		//		l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 15.0f);
		//		l->lightData.positionPower.w = 0.4f;
		//		light->addComponent<Component::TransformationRegister>("last-light-" + std::to_string(i));
		//		light->addComponent<Component::EntityPlacable>("last-light-" + std::to_string(i));
		//		col.push_back(light);
		//	}
		//}
		//{
		//	auto e = scene->createEntity();
		//	auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__credits");
		//	if (!meshObj)
		//		return false;
		//	auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
		//	meshComponent->setShader("MaterialBasic");
		//	e->addComponent<Component::TransformationRegister>("credits-text");
		//	e->addComponent<Component::EntityPlacable>("credits-text");
		//	col.push_back(e);
		//}
		//{
		//	auto e = scene->createEntity();
		//	auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__thanks");
		//	if (!meshObj)
		//		return false;
		//	auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
		//	meshComponent->setShader("MaterialBasic");
		//	e->addComponent<Component::TransformationRegister>("thanks-text");
		//	e->addComponent<Component::EntityPlacable>("thanks-text");
		//	col.push_back(e);
		//}
		//{
		//	auto e = scene->createEntity();
		//	auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__using");
		//	if (!meshObj)
		//		return false;
		//	auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
		//	meshComponent->setShader("MaterialBasic");
		//	e->addComponent<Component::TransformationRegister>("using-text");
		//	e->addComponent<Component::EntityPlacable>("using-text");
		//	col.push_back(e);
		//}
		return true;
	}

	bool LastRoom::_disable()
	{
		auto scene = _scene.lock();
		for (auto e : col)
			scene->destroy(e);
		col.clear();
		return true;
	}