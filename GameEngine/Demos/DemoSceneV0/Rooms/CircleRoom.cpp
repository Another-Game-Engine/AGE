#include "CircleRoom.hh"
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
#include <Components/RotationForce.hpp>
#include <Components/SpotLight.hh>

	CircleRoom::CircleRoom(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
	}

	CircleRoom::~CircleRoom()
	{
		auto scene = _scene.lock();
		if (!scene)
			return;
	}

	bool CircleRoom::init()
	{
		// load
		auto s = _scene.lock();
		s->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__dragon.cpd"));
		hotZoneCircleProjection = createHotZone("Circle->Projection", "HZ-projection-circle");
		hotZoneCircleEngine = createHotZone("Circle->Engine", "HZ-circle-engine");
		return true;
	}

	bool CircleRoom::_enable()
	{
		auto scene = _scene.lock();

		{
			dragon = scene->createEntity();
			dragon->setLocalTransform(glm::translate(dragon->getLocalTransform(), glm::vec3(0)));
			dragon->setLocalTransform(glm::scale(dragon->getLocalTransform(), glm::vec3(1.0f)));
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__dragon");
			if (!meshObj)
				return false;
			auto meshComponent = dragon->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			auto rigidBody = dragon->addComponent<Component::RigidBody>(0.0f);
			rigidBody->setMass(0.0f);
			rigidBody->setCollisionShape(Component::RigidBody::MESH, "collision_shape_static_dragon");
			rigidBody->getBody().setFlags(COLLISION_LAYER_STATIC);
			rigidBody->getShape().setMargin(0.001f);
			rigidBody->getBody().setFriction(1.0f);
			rigidBody->getBody().setRestitution(0.9f);

//			dragon->addComponent<Component::EntityPlacable>("mesh-dragon");
			dragon->addComponent<Component::TransformationRegister>("mesh-dragon");
		}

		{
			rotationAxis = scene->createEntity();
			auto rotForce = rotationAxis->addComponent<Component::RotationForce>();
			rotForce->force = glm::vec3(0.0f, 10.0f, 0.0f);
			//auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__cube");
			//rotationAxis->addComponent<Component::MeshRenderer>(meshObj)->setShader("MaterialBasic");
//			rotationAxis->addComponent<Component::EntityPlacable>("circle-room-rotation-axi-1");
			rotationAxis->addComponent<Component::TransformationRegister>("circle-room-rotation-axis-1");
		}
		{
				auto spotLight = scene->createEntity();
				rotationAxis->addChild(spotLight);
				auto l = spotLight->addComponent<Component::SpotLight>();
				l->lightData.colorRange = glm::vec4(0.909f, 0.458f, 0.447f, 100.0f);
				l->lightData.positionPower.w = 3.0f;
				l->projection = glm::perspective(120.0f, 1.0f, 0.1f, 100.0f);
				l->lightData.shadowId = 1;
//				spotLight->addComponent<Component::EntityPlacable>("circle-room-spotlight-1");
				spotLight->addComponent<Component::TransformationRegister>("circle-room-spotlight-1");
				spotLights.push_back(spotLight);
		}
		{
			auto spotLight = scene->createEntity();
			rotationAxis->addChild(spotLight);
			auto l = spotLight->addComponent<Component::SpotLight>();
			l->lightData.colorRange = glm::vec4(1.0f, 0.5333f, 0.0666f, 100.0f);
			l->lightData.positionPower.w = 1.0f;
			l->projection = glm::perspective(120.0f, 1.0f, 0.1f, 100.0f);
			l->lightData.shadowId = 1;
//			spotLight->addComponent<Component::EntityPlacable>("circle-room-spotlight-2");
			spotLight->addComponent<Component::TransformationRegister>("circle-room-spotlight-2");
			spotLights.push_back(spotLight);
		}
		{
			auto spotLight = scene->createEntity();
			rotationAxis->addChild(spotLight);
			auto l = spotLight->addComponent<Component::SpotLight>();
			l->lightData.colorRange = glm::vec4(0.6980f, 0.341f, 0.305f, 100.0f);
			l->lightData.positionPower.w = 2.0f;
			l->projection = glm::perspective(120.0f, 1.0f, 0.1f, 100.0f);
			l->lightData.shadowId = 1;
//			spotLight->addComponent<Component::EntityPlacable>("circle-room-spotlight-3");
			spotLight->addComponent<Component::TransformationRegister>("circle-room-spotlight-3");
			spotLights.push_back(spotLight);
		}
		{
			whyAge = scene->createEntity();
			auto meshObj = scene->getInstance<AssetsManager>()->get<ObjFile>("obj__why-age");
			if (!meshObj)
				return false;
			auto meshComponent = whyAge->addComponent<Component::MeshRenderer>(meshObj);
			meshComponent->setShader("MaterialBasic");
			whyAge->addComponent<Component::TransformationRegister>("why-age-text");
			whyAge->addComponent<Component::EntityPlacable>("why-age-text");
		}
		{
			auto light = scene->createEntity();
			auto l = light->addComponent<Component::PointLight>();
			l->lightData.colorRange = glm::vec4(1.0f, 1.0f, 1.0f, 20.0f); // distance
			l->lightData.positionPower.w = 0.9f; // intensite
			light->addComponent<Component::TransformationRegister>("circle-room-corridor-light");
			light->addComponent<Component::EntityPlacable>("circle-room-corridor-light");
			corridorLight = light;
		}



		return true;
	}

	bool CircleRoom::_disable()
	{
		auto scene = _scene.lock();
		scene->destroy(dragon);
		scene->destroy(rotationAxis);
		for (auto e : spotLights)
			scene->destroy(e);
		spotLights.clear();
		scene->destroy(whyAge);
		scene->destroy(corridorLight);
		return true;
	}