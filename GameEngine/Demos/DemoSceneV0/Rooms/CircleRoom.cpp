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

			dragon->addComponent<Component::EntityPlacable>("mesh-dragon");
			dragon->addComponent<Component::TransformationRegister>("mesh-dragon");
		}

		return true;
	}

	bool CircleRoom::_disable()
	{
		auto scene = _scene.lock();
		scene->destroy(dragon);
		return true;
	}