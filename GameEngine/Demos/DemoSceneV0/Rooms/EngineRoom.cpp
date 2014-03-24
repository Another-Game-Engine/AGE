#include "EngineRoom.hh"
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

	EngineRoom::EngineRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
		, fboAsteroidReceiver(scene.lock()->getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>())
		, fboAsteroidId(0)
		, fboSponzaReceiver(scene.lock()->getDependenciesInjectorParent().lock()->getInstance<PubSub::Manager>())
		, fboSponzaId(0)
	{
	}

	EngineRoom::~EngineRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool EngineRoom::init()
	{
		// load
		auto s = _scene.lock();
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__Cat.cpd"));
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__3DTexts.cpd"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/TextsEngine.CPDAnimation"));
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__spiral.cpd"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/fboasteroid.CPDAnimation"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/fbosolar.CPDAnimation"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/fbosponza.CPDAnimation"));


		hotZoneEngineCircle = createHotZone("Engine->Circle", "HZ-circle-engine");
		hotZoneEngineLast = createHotZone("Engine->Last", "HZ-engine-last");

		///
		/// Asteroid Demo FBO

		{
			auto &_id = fboAsteroidId;
			fboAsteroidReceiver.globalSub(PubSubKey("fboAsteroidSystemId"), [&_id](GLuint id) {
				_id = id;
			});
		}
		{
			auto &_sid = fboSponzaId;
			fboSponzaReceiver.globalSub(PubSubKey("fboSponzaId"), [&_sid](GLuint id) {
				_sid = id;
			});
		}


		return true;
	}

	bool EngineRoom::_enable()
	{
		auto scene = _scene.lock();

		//
		// Point light
		//

		for (auto i = 0; i < 8; ++i)
		{
			auto e = scene->createEntity();
			auto l = e->addComponent<Component::PointLight>();
			l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 30.0f); // distance
			l->lightData.positionPower.w = 2.0f; // intensite
			e->addComponent<Component::TransformationRegister>("engine-room-pointlight-" + std::to_string(i));
	//		e->addComponent<Component::EntityPlacable>("engine-room-pointlight-" + std::to_string(i));
			map["engine-room-pointlight-" + std::to_string(i)] = e;
		}

		///
		/// CAT
		///
		{
			auto cat = scene->createEntity();
			cat->setLocalTransform(glm::translate(cat->getLocalTransform(), glm::vec3(0)));
			cat->setLocalTransform(glm::scale(cat->getLocalTransform(), glm::vec3(1.0f)));
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cat");
			if (!meshObj)
				return false;
			auto meshComponent = cat->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			auto rigidBody = cat->addComponent<Component::RigidBody>(0.0f);
			rigidBody->setMass(0.0f);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_cat");
			rigidBody->getBody().setFlags(COLLISION_LAYER_STATIC);
			rigidBody->getShape().setMargin(0.001f);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(0.9f);

		//	cat->addComponent<Component::EntityPlacable>("engine-room-gros-chat");
			cat->addComponent<Component::TransformationRegister>("engine-room-gros-chat");
			map["cat"] = cat;
		}

		//
		// SORRY
		//
		{
		//	auto e = scene->createEntity();
		//	auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__sorry");
		//	if (!meshObj)
		//		return false;
		//	auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
		//	meshComponent->setShader("MaterialBasic");

		////	e->addComponent<Component::EntityPlacable>("engine-room-sorry");
		//	e->addComponent<Component::TransformationRegister>("engine-room-sorry");
		//	map["sorry"] = e;
		}

		//
		// COMPONENT 3D
		//
		{
			auto e = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__component");
			if (!meshObj)
				return false;
			auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");

			e->addComponent<Component::EntityPlacable>("engine-room-component-title");
			e->addComponent<Component::TransformationRegister>("engine-room-component-title");
			map["component"] = e;
		}

		//
		// SYSTEM 3D
		//
		{
			auto e = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__system");
			if (!meshObj)
				return false;
			auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");

			//	e->addComponent<Component::EntityPlacable>("engine-room-sorry");
			e->addComponent<Component::TransformationRegister>("engine-room-system-title");
			//e->addComponent<Component::EntityPlacable>("engine-room-system-title");
			map["system"] = e;
		}

		//
		// ENTITY 3D
		//
		{
			auto e = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__entity");
			if (!meshObj)
				return false;
			auto meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");

			//	e->addComponent<Component::EntityPlacable>("engine-room-sorry");
			e->addComponent<Component::TransformationRegister>("engine-room-entity-title");
			//e->addComponent<Component::EntityPlacable>("engine-room-entity-title");
			map["entity"] = e;
		}

		//
		//
		//

		//
		// Age intro
		//
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("TextsEngine", "engine_desc"));
			sprite->delay = 0.0f;
			e->addComponent<Component::TransformationRegister>("engine-desc-text-sprite");
			//e->addComponent<Component::EntityPlacable>("engine-desc-text-sprite");
			map["engine-desc-text-sprite"] = e;
		}

		//
		// Age system
		//
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("TextsEngine", "system"));
			sprite->delay = 0.0f;
			e->addComponent<Component::TransformationRegister>("system-code-sprite");
			//e->addComponent<Component::EntityPlacable>("system-code-sprite");
			map["system-code-sprite"] = e;
		}

		//
		// Age entity
		//
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("TextsEngine", "entity"));
			sprite->delay = 0.0f;
			e->addComponent<Component::TransformationRegister>("entity-code-sprite");
			//e->addComponent<Component::EntityPlacable>("entity-code-sprite");
			map["entity-code-sprite"] = e;
		}

		//
		// Age component
		//
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("TextsEngine", "component"));
			sprite->delay = 0.0f;
			e->addComponent<Component::TransformationRegister>("component-code-sprite");
			//e->addComponent<Component::EntityPlacable>("component-code-sprite");
			map["component-code-sprite"] = e;
		}

		//
		// Age Logo
		//
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("TextsEngine", "age"));
			sprite->delay = 0.0f;
			e->addComponent<Component::TransformationRegister>("logo-sprite");
			//e->addComponent<Component::EntityPlacable>("logo-sprite");
			map["logo-sprite"] = e;
		}

		//
		// Age multiscene
		//
		{
			auto e = scene->createEntity();
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("TextsEngine", "multiscene"));
			sprite->delay = 0.0f;
			e->addComponent<Component::TransformationRegister>("multiscene-text-sprite");
			//e->addComponent<Component::EntityPlacable>("multiscene-text-sprite");
			map["multiscene-text-sprite"] = e;
		}
		// Spiral room sponza
		{
			auto room = scene->createEntity();
			room->addComponent<Component::TransformationRegister>("spiral-sub-room-sponza");
			room->addComponent<Component::EntityPlacable>("spiral-sub-room-sponza");
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__spiral");
			if (!meshObj)
				return false;
			auto meshComponent = room->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			auto rigidBody = room->addComponent<Component::RigidBody>(0.0f);
			rigidBody->setMass(0);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_spiral");
			//	rigidBody->getShape().setMargin(0.001f);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(0.9f);
			map["spiral-sub-room-sponza"] = room;

			auto e = scene->createEntity();
			meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__sponza-text-3d");
			if (!meshObj)
				return false;
			meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			e->addComponent<Component::TransformationRegister>("spiral-sponza-room-title");
			e->addComponent<Component::EntityPlacable>("spiral-sponza-room-title");
			map["spiral-sponza-room-title"] = e;
		}

		// Spiral room solarSystem
		{
			auto room = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__spiral");
			if (!meshObj)
				return false;
			auto meshComponent = room->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			auto rigidBody = room->addComponent<Component::RigidBody>(0.0f);
			rigidBody->setMass(0);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_spiral");
			//	rigidBody->getShape().setMargin(0.001f);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(0.9f);
			room->addComponent<Component::TransformationRegister>("spiral-sub-room-solar");
			room->addComponent<Component::EntityPlacable>("spiral-sub-room-solar");
			map["spiral-sub-room-solar"] = room;

			auto e = scene->createEntity();
			meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__solarSystem-text");
			if (!meshObj)
				return false;
			meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			e->addComponent<Component::TransformationRegister>("spiral-solar-room-title");
			e->addComponent<Component::EntityPlacable>("spiral-solar-room-title");
			map["spiral-solar-room-title"] = e;
		}

		// Spiral room asteroid
		{
			auto room = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__spiral");
			if (!meshObj)
				return false;
			auto meshComponent = room->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			auto rigidBody = room->addComponent<Component::RigidBody>(0.0f);
			rigidBody->setMass(0);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_spiral");
			//	rigidBody->getShape().setMargin(0.001f);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(0.9f);
			room->addComponent<Component::TransformationRegister>("spiral-sub-room-asteroid");
			room->addComponent<Component::EntityPlacable>("spiral-sub-room-asteroid");
			map["spiral-sub-room-asteroid"] = room;

			auto e = scene->createEntity();
			meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__asteroid-text");
			if (!meshObj)
				return false;
			meshComponent = e->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			e->addComponent<Component::TransformationRegister>("spiral-asteroid-room-title");
			e->addComponent<Component::EntityPlacable>("spiral-asteroid-room-title");
			map["spiral-asteroid-room-title"] = e;
		}

		// fbo asteroid
		{
			scene->getInstance<SceneManager>()->enableScene("Asteroid", 1);
			auto e = scene->createEntity();
			e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(-8, 1, 0)));
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("FBO-asteroid", "asteroid"));
			sprite->delay = 1.0f / 10.0f;
			sprite->animation->getMaterial().diffuseTex->id = fboAsteroidId;
			sprite->animation->_frames[0]->_uvs = glm::vec4(1, 0, 0, 1);
			sprite->animation->_frames[0]->load(scene->getInstance < VertexManager<4> >());
			e->addComponent<Component::TransformationRegister>("fbo-asteroid");
			e->addComponent<Component::EntityPlacable>("fbo-asteroid");
			map["fbo-asteroid"] = e;
		}

		// fbo sponza
		{
			scene->getInstance<SceneManager>()->enableScene("Sponza", 2);
			auto e = scene->createEntity();
			e->setLocalTransform(glm::translate(e->getLocalTransform(), glm::vec3(-8, 1, 0)));
			auto sprite = e->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("FBO-sponza", "sponza"));
			sprite->delay = 1.0f / 10.0f;
			sprite->animation->getMaterial().diffuseTex->id = fboSponzaId;
			sprite->animation->_frames[0]->_uvs = glm::vec4(1, 0, 0, 1);
			sprite->animation->_frames[0]->load(scene->getInstance < VertexManager<4> >());
			e->addComponent<Component::TransformationRegister>("fbo-sponza");
			e->addComponent<Component::EntityPlacable>("fbo-sponza");
			map["fbo-sponza"] = e;
		}
		return true;
	}

	bool EngineRoom::_disable()
	{
		auto scene = _scene.lock();
		scene->getInstance<SceneManager>()->disableScene("Asteroid");

		for (auto &e : map)
		{
			scene->destroy(e.second);
		}
		return true;
	}