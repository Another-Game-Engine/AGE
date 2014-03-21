#include "Entrance.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>
#include <Sprite/SpriteManager.hh>
#include <Components/SpriteComponent.hh>
#include <Systems/EntityPlacingSystem.hpp>
#include <Systems/TransformationRegisterSystem.hpp>

	Entrance::Entrance(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
		// load
		auto s = scene.lock();
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__demoMuseum.cpd"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/Pong.CPDAnimation"));
		s->getInstance<SpriteManager>()->loadFile(File("../../Assets/Serialized/Trololo.CPDAnimation"));
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
			rigidBody->getBody().setFlags(COLLISION_LAYER_STATIC);
			rigidBody->getShape().setMargin(0.001f);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(0.9f);
		}
		{
			pong = scene->createEntity();
			pong->setLocalTransform(glm::translate(pong->getLocalTransform(), glm::vec3(-8, 1, 0)));
			pong->setLocalTransform(glm::scale(pong->getLocalTransform(), glm::vec3(0.01)));
			auto sprite = pong->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Pong", "pong"));
			sprite->delay = 1.0f / 10.0f;
			pong->addComponent<Component::TransformationRegister>("pong-tableau");
		}
		{
			trololo = scene->createEntity();
			trololo->setLocalTransform(glm::translate(trololo->getLocalTransform(), glm::vec3(-8, 1, 0)));
			trololo->setLocalTransform(glm::scale(trololo->getLocalTransform(), glm::vec3(0.01)));
			auto sprite = trololo->addComponent<Component::Sprite>(scene->getInstance<SpriteManager>()->getAnimation("Trololo", "tmp"));
			sprite->delay = 1.0f / 10.0f;
			scene->getSystem<EntityPlacingSystem>()->setEntity(trololo);
			trololo->addComponent<Component::TransformationRegister>("trololo-tableau");
		}
	}

	bool Entrance::_enable()
	{
		return true;
	}

	bool Entrance::_disable()
	{
		return true;
	}