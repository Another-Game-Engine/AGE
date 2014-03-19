#include "Entrance.hh"
#include <MediaFiles/AssetsManager.hpp>
#include <Components/MeshRenderer.hh>
#include <glm/gtc/matrix_transform.hpp>
#include <Components/RigidBody.hpp>

	Entrance::Entrance(std::weak_ptr<AScene> scene)
		: Room(scene)
	{
	}

	Entrance::~Entrance()
	{
		auto scene = _scene.lock();
		if (scene)
			scene->destroy(room);
	}

	bool Entrance::init()
	{
		room = _scene.lock()->createEntity();
		room->setLocalTransform(glm::translate(room->getLocalTransform(), glm::vec3(0)));
		room->setLocalTransform(glm::scale(room->getLocalTransform(), glm::vec3(10.0f)));
		_scene.lock()->getInstance<AssetsManager>()->loadFromList(File("../../Assets/Serialized/export__demoMuseum.cpd"));
		auto meshObj = _scene.lock()->getInstance<AssetsManager>()->get<ObjFile>("obj__demoMuseum");
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

	bool Entrance::_enable()
	{
		return true;
	}

	bool Entrance::_disable()
	{
		return true;
	}