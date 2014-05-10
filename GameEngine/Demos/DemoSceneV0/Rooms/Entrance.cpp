#include "Entrance.hh"


	Entrance::Entrance(std::weak_ptr<AScene> &&scene)
		: Room(std::move(scene))
		, framebufferIdReceiver(scene.lock()->getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>())
	{
	}

	Entrance::~Entrance()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
		scene->destroy(room);
		scene->destroy(pong);
	}

	bool Entrance::init()
	{
		// load
		auto s = _scene.lock();
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/Pong.CPDAnimation"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/SceneReader.CPDAnimation"));
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__3DTexts.cpd"));
		hotZone = createHotZone("Entrance->Projection", "HZ-entrance-projection");

		return true;
	}

	bool Entrance::_enable()
	{
		auto scene = _scene.lock();
		{
			pong = scene->createEntity();
			auto transform = scene->getLocalTransform(pong);
			transform  = glm::translate(transform, glm::vec3(-8, 1, 0));
			transform = glm::scale(transform, glm::vec3(0.01));
			scene->setLocalTransform(pong, transform);
			auto sprite = scene->addComponent<Component::Sprite>(pong, scene->getInstance<SpriteManager>()->getAnimation("Pong", "pong"));
			sprite->delay = 1.0f / 10.0f;
			scene->addComponent<Component::TransformationRegister>(pong, "pong-tableau");
			scene->addComponent<Component::EntityPlacable>(pong, "pong-tableau");
		}
		{
			for (auto i = 0; i < 10; ++i)
			{
				auto light = scene->createEntity();
				auto l = scene->addComponent<Component::PointLight>(light);
				l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 15.0f);
				l->lightData.positionPower.w = 1.0f;
				scene->addComponent<Component::TransformationRegister>(light, "entrance-light-" + std::to_string(i));
				scene->addComponent<Component::EntityPlacable>(light, "entrance-light-" + std::to_string(i));
				lights.push_back(light);
			}
		}
		{
			welcomeText = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__welcome");
			if (!meshObj)
				return false;
			auto meshComponent = scene->addComponent<Component::MeshRenderer>(welcomeText, meshObj);
			meshComponent->setShader("MaterialBasic");
			scene->addComponent<Component::TransformationRegister>(welcomeText, "welcome-text");
			scene->addComponent<Component::EntityPlacable>(welcomeText, "welcome-text");
		}
		return true;
	}

	bool Entrance::_disable()
	{
		auto scene = _scene.lock();
		scene->destroy(pong);
		for (auto e : lights)
		{
			scene->destroy(e);
		}
		lights.clear();
		scene->destroy(welcomeText);
		return true;
	}