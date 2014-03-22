#include "Entrance.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>
#include <Components/CollisionBody.hpp>
#include <Sprite/SpriteManager.hh>
#include <Components/SpriteComponent.hh>
#include <Systems/EntityPlacingSystem.hpp>
#include <Systems/TransformationRegisterSystem.hpp>
#include <Components/PointLight.hh>
#include <Systems/HotZoneSystem.hpp>

	Entrance::Entrance(std::weak_ptr<AScene> scene)
		: Room(scene)
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
			hotZone = scene->createEntity();
			auto rb = hotZone->addComponent<Component::CollisionBody>();
			rb->setCollisionShape(Component::CollisionBody::BOX);
			//rb->getBody().getBroadphaseHandle()->m_collisionFilterGroup = COL_HOTZONE;
			//rb->getBody().getBroadphaseHandle()->m_collisionFilterMask = COL_CHARACTER ;
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");
			if (!meshObj)
				return false;
			auto meshComponent = hotZone->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			hotZone->addComponent<Component::HotZone>("entrance-room-hot-zone", "switch-hz-engine-room__entrance", shared_from_this());
			hotZone->addComponent<Component::EntityPlacable>("entrance-room-hot-zone");
			hotZone->addComponent<Component::TransformationRegister>("entrance-room-hot-zone");
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
				l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 6.0f);
				l->lightData.positionPower.w = 2.0f;
				light->addComponent<Component::EntityPlacable>("entrance-light-" + std::to_string(i));
				light->addComponent<Component::TransformationRegister>("entrance-light-" + std::to_string(i));
				lights.push_back(light);
			}
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
		return true;
	}