#include "Entrance.hh"


	Entrance::Entrance(std::weak_ptr<AScene> scene)
		: Room(scene)
		, framebufferIdReceiver(scene.lock()->getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>())
	{
	}

	Entrance::~Entrance()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
		scene->destroy(room);
		scene->destroy(trololo);
		scene->destroy(pong);
	}

	bool Entrance::init()
	{
		// load
		auto s = _scene.lock();
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__demoMuseum.cpd"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/Pong.CPDAnimation"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/Trololo.CPDAnimation"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/SceneReader.CPDAnimation"));
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__3DTexts.cpd"));

		auto scene = _scene.lock();
		{
			room = scene->createEntity();
			room->setLocalTransform(glm::translate(room->getLocalTransform(), glm::vec3(0)));
			room->setLocalTransform(glm::scale(room->getLocalTransform(), glm::vec3(150.0f)));
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__demoMuseum");
			if (!meshObj)
				return false;
			auto meshComponent = room->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			auto rigidBody = room->addComponent<Component::RigidBody>(0.0f);
			rigidBody->setMass(0);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_demoMuseum");
		//	rigidBody->getShape().setMargin(0.001f);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(0.9f);
		}
		hotZone = createHotZone("Entrance->Projection", "HZ-entrance-projection");


		////////
		////
		/// FBO id receiver
		{
			//auto e = scene->createEntity();
			//e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(-8, 1, 0)));
			//auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("shoot", "shoot"));
			//sprite->delay = 1.0f / 10.0f;
			//e->addComponent<Component::TransformationRegister>("test-fb-tableau");
			//e->addComponent<Component::EntityPlacable>("test-fb-tableau");
			//fboTest = e;
			//auto &_id = sprite->animation->getMaterial().ambientTex->id;
			//framebufferIdReceiver.globalSub(PubSubKey("myTextureKeyIs"), [&_id](GLuint id) {
			//	_id = id;
			//});
		}

		return true;
	}

	bool Entrance::_enable()
	{
		auto scene = _scene.lock();
		{
			pong = scene->createEntity();
			pong->setLocalTransform(glm::translate(pong->getLocalTransform(), glm::vec3(-8, 1, 0)));
			pong->setLocalTransform(glm::scale(pong->getLocalTransform(), glm::vec3(0.01)));
			auto sprite = pong->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Pong", "pong"));
			sprite->delay = 1.0f / 10.0f;
			pong->addComponent<Component::TransformationRegister>("pong-tableau");
			pong->addComponent<Component::EntityPlacable>("pong-tableau");
		}
		{
			trololo = scene->createEntity();
			trololo->setLocalTransform(glm::translate(trololo->getLocalTransform(), glm::vec3(-8, 1, 0)));
			trololo->setLocalTransform(glm::scale(trololo->getLocalTransform(), glm::vec3(0.01)));
			auto sprite = trololo->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Trololo", "tmp"));
			sprite->delay = 1.0f / 10.0f;
			trololo->addComponent<Component::TransformationRegister>("trololo-tableau");
			trololo->addComponent<Component::EntityPlacable>("trololo-tableau");
		}
		{
			for (auto i = 0; i < 10; ++i)
			{
				auto light = scene->createEntity();
				auto l = light->addComponent<Component::PointLight>();
				l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 15.0f);
				l->lightData.positionPower.w = 1.0f;
				light->addComponent<Component::TransformationRegister>("entrance-light-" + std::to_string(i));
				light->addComponent<Component::EntityPlacable>("entrance-light-" + std::to_string(i));
				lights.push_back(light);
			}
		}
		{
			welcomeText = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__welcome");
			if (!meshObj)
				return false;
			auto meshComponent = welcomeText->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			welcomeText->addComponent<Component::TransformationRegister>("welcome-text");
			welcomeText->addComponent<Component::EntityPlacable>("welcome-text");
		}
		return true;
	}

	bool Entrance::_disable()
	{
		auto scene = _scene.lock();
		scene->destroy(pong);
		scene->destroy(trololo);
		for (auto e : lights)
		{
			scene->destroy(e);
		}
		lights.clear();
		scene->destroy(welcomeText);
		return true;
	}