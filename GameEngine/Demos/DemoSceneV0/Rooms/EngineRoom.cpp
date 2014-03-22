#include "EngineRoom.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>
#include <Sprite/SpriteManager.hh>
#include <Components/SpriteComponent.hh>
#include <Systems/EntityPlacingSystem.hpp>
#include <Systems/TransformationRegisterSystem.hpp>
#include <Components/SpaceshipController.hpp>

	EngineRoom::EngineRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
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
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__House.cpd"));
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__dragon.cpd"));
//		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__galileo.cpd"));

		auto scene = _scene.lock();
		{
		}

		return true;
	}

	bool EngineRoom::_enable()
	{
		auto scene = _scene.lock();
		{
			cat = scene->createEntity();
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

			cat->addComponent<Component::EntityPlacable>("engine-room-gros-chat");
			cat->addComponent<Component::TransformationRegister>("engine-room-gros-chat");
		}
		{
			house = scene->createEntity();
			house->setLocalTransform(glm::translate(house->getLocalTransform(), glm::vec3(0)));
			house->setLocalTransform(glm::scale(house->getLocalTransform(), glm::vec3(1.0f)));
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__dragon");
			if (!meshObj)
				return false;
			auto meshComponent = house->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			auto rigidBody = house->addComponent<Component::RigidBody>(0.0f);
			rigidBody->setMass(0.0f);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_dragon");
			rigidBody->getBody().setFlags(COLLISION_LAYER_STATIC);
			rigidBody->getShape().setMargin(0.001f);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(0.9f);

			house->addComponent<Component::EntityPlacable>("medieval-house");
			house->addComponent<Component::TransformationRegister>("medieval-house");
		}
//		{
//			pong = scene->createEntity();
//			pong->setLocalTransform(glm::translate(pong->getLocalTransform(), glm::vec3(-8, 1, 0)));
//			pong->setLocalTransform(glm::scale(pong->getLocalTransform(), glm::vec3(0.01)));
//			auto sprite = pong->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Pong", "pong"));
//			sprite->delay = 1.0f / 10.0f;
//			pong->addComponent<Component::TransformationRegister>("pong-tableau");
//		}
//		{
//			trololo = scene->createEntity();
//			trololo->setLocalTransform(glm::translate(trololo->getLocalTransform(), glm::vec3(-8, 1, 0)));
//			trololo->setLocalTransform(glm::scale(trololo->getLocalTransform(), glm::vec3(0.01)));
//			auto sprite = trololo->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Trololo", "tmp"));
//			sprite->delay = 1.0f / 10.0f;
////			scene->getSystem<EntityPlacingSystem>()->setEntity(trololo);
//			trololo->addComponent<Component::TransformationRegister>("trololo-tableau");
//		}
//		{
//
//		}
		return true;
	}

	bool EngineRoom::_disable()
	{
		auto scene = _scene.lock();
		scene->destroy(cat);
		scene->destroy(house);
		return true;
	}